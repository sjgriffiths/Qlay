using System.Diagnostics;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

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
            DataContext = new DataModel(FindName("CircuitScrollViewer") as ContentControl);
        }

        private void New_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void New_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            //Reset the DataModel
            ((DataModel)DataContext).New();
        }

        private void Open_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void Open_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            //Show a file dialog and load the selected file into the DataModel
            Microsoft.Win32.OpenFileDialog openFileDialog = new Microsoft.Win32.OpenFileDialog
            {
                Filter = "Qlay Visual circuits (*.qvc)|*.qvc|All files (*.*)|*.*"
            };

            if (openFileDialog.ShowDialog() == true)
            {
                string path = openFileDialog.FileName;

                using (FileStream fs = File.Open(path, FileMode.Open, FileAccess.Read))
                {
                    ((DataModel)DataContext).Deserialise(fs);
                }
                
                ((DataModel)DataContext).FilePath = path;
            }
        }

        private void Save_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void Save_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            //If Untitled, delegate to SaveAs, otherwise just Save
            if (((DataModel)DataContext).FilePath == null)
                SaveAs_Executed(sender, e);
            else
            {
                using (FileStream fs = File.Open(((DataModel)DataContext).FilePath, FileMode.Open, FileAccess.Write))
                using (StreamWriter sw = new StreamWriter(fs))
                {
                    sw.Write(((DataModel)DataContext).Serialise());
                }
            }
        }

        private void SaveAs_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void SaveAs_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            //Show a file dialog, then create and write into the selected file
            Microsoft.Win32.SaveFileDialog saveFileDialog = new Microsoft.Win32.SaveFileDialog
            {
                Filter = "Qlay Visual circuits (*.qvc)|*.qvc|All files (*.*)|*.*"
            };

            if (saveFileDialog.ShowDialog() == true)
            {
                string path = saveFileDialog.FileName;

                using (FileStream fs = File.Create(path))
                using (StreamWriter sw = new StreamWriter(fs))
                {
                    sw.Write(((DataModel)DataContext).Serialise());
                }

                ((DataModel)DataContext).FilePath = path;
            }
        }

        private void Help_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void Help_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            ProcessStartInfo psi = new ProcessStartInfo(@"QLAYVISUAL.chm")
            {
                WindowStyle = ProcessWindowStyle.Maximized
            };
            Process.Start(psi);
        }

        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void About_Click(object sender, RoutedEventArgs e)
        {
            Process.Start("https://github.com/sjgriffiths/Qlay");
        }

        private void RunSimulation_Click(object sender, RoutedEventArgs e)
        {
            int repeats = int.Parse(NumOfRepeats.Text);
            var results = ((DataModel)DataContext).RunSimulation(repeats);
            ((DataModel)DataContext).CircuitCanvas.SetMeasurementLabels(results);
        }
    }
}
