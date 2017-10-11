using Android.App;
using Android.Widget;
using Android.OS;
using Android.Telephony;
using Android.Content;
using Android.Util;
using System;
using Android.Views;
using Android.Preferences;
using ControllerApp.Domain.Activities;

namespace ControllerApp
{
    [Activity(Label = "Sistema")]
    public class ControllerActivity : Activity
    {

        private SMSBroadcastReceiver SmsReceiver { get; set; }


        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.ControllerLayout);

            ISharedPreferences prefs = PreferenceManager.GetDefaultSharedPreferences(this);
            long arduinoPhoneNumber = prefs.GetLong(GetString(Resource.String.controller_number), 0);
            this.SmsReceiver = new SMSBroadcastReceiver(this.UpdateReceived, arduinoPhoneNumber);
            
            ProgressBar tankBar = FindViewById<ProgressBar>(Resource.Id.tankBar);
            tankBar.Max = 100;
            tankBar.Progress = 0;
            tankBar.Visibility = ViewStates.Invisible;

            FindViewById<Button>(Resource.Id.update).Click += (sender, ea) => {
                ShowSendSMSAlertDialog(GetString(Resource.String.message_warning),
                    "Deseja actualizar?", 
                    new SMS(arduinoPhoneNumber, GetString(Resource.String.sms_command_status_report))).Show();
            };

            FindViewById<Button>(Resource.Id.openValve).Click += (sender, ea) => {
                ShowSendSMSAlertDialog(GetString(Resource.String.message_warning),
                    "Deseja abrir a valvula?", 
                    new SMS(arduinoPhoneNumber, GetString(Resource.String.sms_command_open_valve))).Show();
            };

            FindViewById<Button>(Resource.Id.closeValve).Click += (sender, ea) => {
                ShowSendSMSAlertDialog(GetString(Resource.String.message_warning), 
                    "Deseja fechar a valvula?", 
                    new SMS(arduinoPhoneNumber, GetString(Resource.String.sms_command_close_valve))).Show();
            };

            FindViewById<Button>(Resource.Id.turnOnPump).Click += (sender, ea) => {
                ShowSendSMSAlertDialog(GetString(Resource.String.message_warning), 
                    "Deseja ligar a bomba de água?", 
                    new SMS(arduinoPhoneNumber, GetString(Resource.String.sms_command_open_water_pump))).Show();
            };

            FindViewById<Button>(Resource.Id.turnOffPump).Click += (sender, ea) => {
                ShowSendSMSAlertDialog(GetString(Resource.String.message_warning), 
                    "Deseja desligar a bomba de água?", 
                    new SMS(arduinoPhoneNumber, GetString(Resource.String.sms_command_close_water_pump))).Show();
            };

            FindViewById<Button>(Resource.Id.turnOnLamp).Click += (sender, ea) => {
                ShowSendSMSAlertDialog(GetString(Resource.String.message_warning), 
                    "Deseja ligar a lâmpada?", 
                    new SMS(arduinoPhoneNumber, GetString(Resource.String.sms_command_open_lamp))).Show();
            };

            FindViewById<Button>(Resource.Id.turnOffLamp).Click += (sender, ea) => {
                ShowSendSMSAlertDialog(GetString(Resource.String.message_warning), 
                    "Deseja desligar a lâmpada?", 
                    new SMS(arduinoPhoneNumber, GetString(Resource.String.sms_command_close_lamp))).Show();
            };

            FindViewById<Button>(Resource.Id.resetButton).Click += (sender, ea) => {
                var alertDialog = new AlertDialog.Builder(this);
                alertDialog.SetTitle("Reset da Aplicação");
                alertDialog.SetMessage("Deseja fazer reset dos dados e configurações?");
                alertDialog.SetPositiveButton(GetString(Resource.String.button_yes), (a, b) => {
                    ISharedPreferences prefsTemp = PreferenceManager.GetDefaultSharedPreferences(this);
                    ISharedPreferencesEditor editor = prefsTemp.Edit();
                    editor.Clear();
                    editor.Commit();
                    StartActivity(typeof(SetupActivity));
                    Finish();
                    Toast.MakeText(this.BaseContext, "Reset Realizado Com Sucesso", ToastLength.Long).Show();
                });
                alertDialog.SetNegativeButton(GetString(Resource.String.button_no), (a, b) => {
                    Toast.MakeText(this.BaseContext, "Reset Cancelado", ToastLength.Long).Show();
                });
                alertDialog.SetNeutralButton(GetString(Resource.String.button_cancel), (a, b) => {
                    Toast.MakeText(this.BaseContext, "Reset Cancelado", ToastLength.Long).Show();
                });
                alertDialog.Show();
            };

        }

        protected override void OnPause()
        {
            base.OnPause();
            this.UnregisterReceiver(this.SmsReceiver);
        }

        protected override void OnResume()
        {
            base.OnResume();
            this.RegisterReceiver(this.SmsReceiver, new IntentFilter(SMSBroadcastReceiver.INTENT_ACTION));
            LoadLastDataToUI();
        }

        #region Private methods

        private void LoadLastDataToUI()
        {
            ISharedPreferences prefs = PreferenceManager.GetDefaultSharedPreferences(this);
            string temp = prefs.GetString("LastUpdate", null);
            if (temp != null) { FindViewById<TextView>(Resource.Id.labelLastUpdate).Text = temp; }
            temp = prefs.GetString("Valve", null);
            if (temp != null) { FindViewById<TextView>(Resource.Id.labelValve).Text = temp; }
            temp = prefs.GetString("WaterPump", null);
            if (temp != null) { FindViewById<TextView>(Resource.Id.labelWaterPump).Text = temp; }
            temp = prefs.GetString("Lamp", null);
            if (temp != null) { FindViewById<TextView>(Resource.Id.labelLamp).Text = temp; }
            temp = prefs.GetString("Capacity", null);
            if (temp != null)
            {
                ProgressBar tankBar = FindViewById<ProgressBar>(Resource.Id.tankBar);
                tankBar.Progress = int.Parse(temp);
                tankBar.Visibility = ViewStates.Visible;
            }
        }

        private void UpdateReceived(SMS sms)
        {
            const char componentDelimiter = '\n';
            const char fieldDelimiter = ':';
            ISharedPreferences prefs = PreferenceManager.GetDefaultSharedPreferences(this);
            ISharedPreferencesEditor editor = prefs.Edit();

            string currentTimeString = DateTime.Now.ToLocalTime().ToString();
            editor.PutString("LastUpdate", currentTimeString);
            FindViewById<TextView>(Resource.Id.labelLastUpdate).Text = currentTimeString;

            string[] components = sms.Body.Split(componentDelimiter);
            foreach(string c in components)
            {
                string[] fields = c.Split(fieldDelimiter);
                if(fields.Length == 2)
                {
                    if (fields[0].Equals("Valve"))
                    {
                        FindViewById<TextView>(Resource.Id.labelValve).Text = fields[1];
                        editor.PutString("Valve", fields[1]);
                    }
                    else if (fields[0].Equals("WaterPump"))
                    {
                        FindViewById<TextView>(Resource.Id.labelWaterPump).Text = fields[1];
                        editor.PutString("WaterPump", fields[1]);
                    }
                    else if (fields[0].Equals("Lamp"))
                    {
                        FindViewById<TextView>(Resource.Id.labelLamp).Text = fields[1];
                        editor.PutString("Lamp", fields[1]);
                    }
                    else if (fields[0].Equals("Capacity"))
                    {
                        ProgressBar tankBar = FindViewById<ProgressBar>(Resource.Id.tankBar);
                        tankBar.Progress = int.Parse(fields[1]);
                        tankBar.Visibility = ViewStates.Visible;
                        editor.PutString("Capacity", fields[1]);
                    }
                }
            }

            editor.Apply();
        }

        private AlertDialog ShowSendSMSAlertDialog(string title, string message, SMS sms)
        {
            EditText getPinEditText = new EditText(this)
            {
                InputType = Android.Text.InputTypes.NumberVariationPassword
            };
            var alertDialog = new AlertDialog.Builder(this);
            alertDialog.SetTitle(title);
            alertDialog.SetMessage(message);
            alertDialog.SetView(getPinEditText);
            alertDialog.SetPositiveButton(GetString(Resource.String.button_yes), (a, b) => {
                SmsManager smsManager = SmsManager.Default;
                smsManager.SendTextMessage(sms.PhoneNumber.ToString(), null, sms.Body, null, null);
                Toast.MakeText(this.BaseContext, GetString(Resource.String.command_sent), ToastLength.Long).Show();
            });
            alertDialog.SetNegativeButton(GetString(Resource.String.button_no), (a, b) => {
                Toast.MakeText(this.BaseContext, GetString(Resource.String.command_canceled), ToastLength.Long).Show();
            });
            alertDialog.SetNeutralButton(GetString(Resource.String.button_cancel), (a, b) => {
                Toast.MakeText(this.BaseContext, GetString(Resource.String.command_canceled), ToastLength.Long).Show();
            });
            return alertDialog.Show();
        }

        #endregion

    }
}

