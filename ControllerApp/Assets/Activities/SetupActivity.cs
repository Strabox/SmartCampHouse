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
using Android.Preferences;
using Android.Accounts;

namespace ControllerApp.Domain.Activities
{
    [Activity(Label = "Controlador Serra", MainLauncher = true)]
    public class SetupActivity : Activity
    {
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);

            ISharedPreferences prefs = PreferenceManager.GetDefaultSharedPreferences(this);
            if(prefs.GetBoolean(GetString(Resource.String.setup_done), false))
            {
                StartActivity(typeof(ControllerActivity));
                Finish();
                return;
            }
            SetContentView(Resource.Layout.SetupLayout);

            FindViewById<Button>(Resource.Id.submitButton).Click += (sender, ea) => {
                EditText phoneNumber = FindViewById<EditText>(Resource.Id.phoneNumber);
                ISharedPreferencesEditor editor = prefs.Edit();
                editor.PutLong(GetString(Resource.String.controller_number), long.Parse(phoneNumber.Text));
                editor.PutBoolean(GetString(Resource.String.setup_done), true);
                editor.Apply();


                StartActivity(typeof(ControllerActivity));
                Finish();
                return;
            };

        }
    }
}