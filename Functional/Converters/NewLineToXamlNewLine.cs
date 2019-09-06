﻿using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace WPF_Cross.Functional.Converters
{
    public class NewLineToXamlNewLine : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value == null) return "";
            string In = (string)value;
            string Out = In.Replace("\n", "&#x0a;");
            return Out;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value == null) return "";
            string Out = (string)value;
            string In = Out.Replace("&#x0a;", "\n");
            return In;
        }
    }
}
