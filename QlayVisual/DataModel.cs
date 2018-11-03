using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Markup;

using qlay.cli;

namespace QlayVisual
{
    /// <summary>
    /// Contains and organises the serialisable circuit data
    /// </summary>
    public class DataModel : INotifyPropertyChanged
    {
        public DataModel(ContentControl contentContainer)
        {
            ContentContainer = contentContainer;
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private void OnPropertyChanged(string name)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }

        /// <summary>
        /// References the owning ContentControl of the XAML data model
        /// </summary>
        public ContentControl ContentContainer { set; get; }

        private string _filePath;

        /// <summary>
        /// Contains the currently loaded QVC file
        /// </summary>
        public string FilePath
        {
            set
            {
                _filePath = value;
                OnPropertyChanged("FilePath");
                OnPropertyChanged("FileName");
                OnPropertyChanged("WindowTitle");
            }
            get { return _filePath; }
        }

        /// <summary>
        /// Currently loaded QVC file calculated from FilePath
        /// </summary>
        public string FileName => Path.GetFileName(FilePath);

        /// <summary>
        /// Title of the window, showing the loaded FileName
        /// </summary>
        public string WindowTitle => (FileName ?? "Untitled") + " - Qlay Visual";


        /// <summary>
        /// Serialises the data model into a storable format
        /// </summary>
        /// <returns></returns>
        public string Serialise()
        {
            return XamlWriter.Save(ContentContainer.Content);
        }

        /// <summary>
        /// Deserialises the data model from a stream
        /// </summary>
        /// <param name="data"></param>
        public void Deserialise(Stream data)
        {
            ContentContainer.Content = XamlReader.Load(data) as CircuitCanvas;
        }

        /// <summary>
        /// Resets the model to begin working on a fresh file
        /// </summary>
        public void New()
        {
            ContentContainer.Content = new CircuitCanvas();
            FilePath = null;
        }

        /// <summary>
        /// Runs the quantum simulation, translating the CircuitCanvas into an experiment
        /// </summary>
        public void RunSimulation()
        {
            CircuitCanvas cc = ContentContainer.Content as CircuitCanvas;

            Core.init();

            int repeats = 1000;
            int zeroes = 0, ones = 0;

            for (int i = 0; i < repeats; i++)
            {
                using (Qubit q = new Qubit())
                {
                    //Apply logic gates in the order they appear, i.e. by X value
                    foreach (CircuitItem ci in cc.Children.OfType<CircuitItem>().OrderBy(n => Canvas.GetLeft(n)))
                    {
                        //List of arguments to pass to logic gate
                        List<object> args = new List<object>();

                        //Obtain angle parameter, if one exists
                        foreach (UIElement uie in ((Canvas)ci.Content).Children)
                            if (uie is TextBox tb)
                                args.Add(Core.deg_to_rad(double.Parse(tb.Text)));

                        //Add qubit argument and call
                        args.Add(q);
                        typeof(Gates).GetMethod((string)ci.Tag).Invoke(null, args.ToArray());
                    }

                    //Measure and log result
                    if (Gates.M(q))
                        ones++;
                    else
                        zeroes++;
                }
            }

            MessageBox.Show("ZERO: " + zeroes + "\nONE:  " + ones);
        }
    }
}
