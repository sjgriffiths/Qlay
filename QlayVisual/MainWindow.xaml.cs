using System.IO;
using System.Windows;
using System.Windows.Input;
using System.Windows.Markup;

using qlay.cli;

namespace QlayVisual
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Qlay.init();

            int repeats = 1000;
            int zeroes = 0, ones = 0;

            for (int i = 0; i < repeats; i++)
            {
                using (var q = new Qubit())
                {
                    Gates.H(q);

                    if (Gates.M(q))
                        ones++;
                    else
                        zeroes++;
                }
            }

            MessageBox.Show("ZERO: " + zeroes + "\nONE:  " + ones);
        }

        private void New_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void New_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            MessageBox.Show("New");
        }

        private void Open_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void Open_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog openFileDialog = new Microsoft.Win32.OpenFileDialog
            {
                Filter = "Qlay Visual circuits (*.qvc)|*.qvc|All files (*.*)|*.*"
            };

            if (openFileDialog.ShowDialog() == true)
            {
                string filename = openFileDialog.FileName;

                using (FileStream fs = File.Open(filename, FileMode.Open, FileAccess.Read))
                {
                    Content = XamlReader.Load(fs);
                }
            }
        }

        private void Save_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            SaveAs_CanExecute(sender, e);
        }

        private void Save_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            SaveAs_Executed(sender, e);
        }

        private void SaveAs_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void SaveAs_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            Microsoft.Win32.SaveFileDialog saveFileDialog = new Microsoft.Win32.SaveFileDialog
            {
                Filter = "Qlay Visual circuits (*.qvc)|*.qvc|All files (*.*)|*.*"
            };

            if (saveFileDialog.ShowDialog() == true)
            {
                string filename = saveFileDialog.FileName;
                string xamlString = XamlWriter.Save(Content);

                using (FileStream fs = File.Create(filename))
                using (StreamWriter sw = new StreamWriter(fs))
                {
                    sw.Write(xamlString);
                }
            }
        }

        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }
    }
}
