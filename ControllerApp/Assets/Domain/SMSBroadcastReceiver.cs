using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.Telephony;
using Android.Util;
using Android.Provider;

namespace ControllerApp
{
    /* Delegate that is called by the SMSBroadcastReceiver */
    public delegate void UpdateTask(SMS sms);

    /*
     * Used to register the SMS receiving events/intents
     */
    [BroadcastReceiver(Exported = false)]
    [IntentFilter(new[] { "android.provider.Telephony.SMS_RECEIVED" }, Priority = (int)IntentFilterPriority.HighPriority)]
    public class SMSBroadcastReceiver : BroadcastReceiver
    {
        private const string LOG_TAG = "SMSReceiver";

        public const string INTENT_ACTION = "android.provider.Telephony.SMS_RECEIVED";

        public UpdateTask UpdateTask { set; get; }
        private long FilterNumber { get; set; }

        public SMSBroadcastReceiver()
        {
            this.UpdateTask = null;
            this.FilterNumber = 0;
        }

        public SMSBroadcastReceiver(UpdateTask updateTask, long filterNumber)
        {
            this.UpdateTask = updateTask;
            this.FilterNumber = filterNumber;
        }

        public override void OnReceive(Context context, Intent intent)
        {
            try
            {
                if (intent.Action == INTENT_ACTION)
                {
                    Log.Debug(LOG_TAG, "SMS Received");

                    SmsMessage[] messages = Telephony.Sms.Intents.GetMessagesFromIntent(intent);
                    SMS sms = null;

                    long originatingAddress = long.Parse(messages[0].OriginatingAddress.Substring(4));
                    string messageBody = messages[0].DisplayMessageBody;

                    if (originatingAddress == FilterNumber)
                    {
                        sms = new SMS(originatingAddress, messageBody);
                        Log.Debug(LOG_TAG, "SMS received from a filter number. From: {0} Body: {1}", sms.PhoneNumber,
                            sms.Body);
                    }
                    else
                    {
                        Log.Debug(LOG_TAG, "SMS received from a non filter number: From: {0}", originatingAddress);
                    }

                    if (UpdateTask != null && sms != null)
                    {
                        UpdateTask(sms);
                    }
                }
            }
            catch(Exception e)
            {
                Log.Error(LOG_TAG, e.Message);
            }
        }
    }
}