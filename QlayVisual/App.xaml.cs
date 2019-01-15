using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace QlayVisual
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private void Application_DispatcherUnhandledException(object sender, System.Windows.Threading.DispatcherUnhandledExceptionEventArgs e)
        {
            string message, title = null;

            switch (e.Exception)
            {
                case ArgumentOutOfRangeException eaoore:
                    message = "The simulation could not run due to a structural error(s) in the circuit. Are any gate controls left hanging?";
                    title = "Error(s) in circuit";
                    break;
                case FormatException fe:
                    message = "One or more gate expecting an angle input is missing the argument entirely or in a valid format.";
                    title = "Error(s) in circuit";
                    break;
                default:
                    message = e.Exception.Message;
                    break;
            }

            MessageBox.Show(message, title ?? "Unhandled exception has occurred", MessageBoxButton.OK, MessageBoxImage.Error);
            e.Handled = true;
        }
    }
}
