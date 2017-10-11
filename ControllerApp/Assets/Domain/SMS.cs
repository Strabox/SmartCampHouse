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

namespace ControllerApp
{
    public class SMS
    {

        public long PhoneNumber { get; set; }

        public string Body { get; set; }

        public SMS(long phoneNumber, string body)
        {
            this.PhoneNumber = phoneNumber;
            this.Body = body;
        }

    }
}