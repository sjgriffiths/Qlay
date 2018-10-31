using System.ComponentModel;
using System.IO;
using System.Linq;
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
            int n = cc.Children.OfType<CircuitItem>().Count();

            Core.init();

            int repeats = 1000;
            int zeroes = 0, ones = 0;

            for (int i = 0; i < repeats; i++)
            {
                using (var q = new Qubit())
                {
                    //Apply X gate for each CircuitItem
                    for (int j = 0; j < n; j++)
                        Gates.X(q);

                    //Measure and log result
                    if (Gates.M(q))
                        ones++;
                    else
                        zeroes++;
                }
            }

            System.Windows.MessageBox.Show("ZERO: " + zeroes + "\nONE:  " + ones);
        }
    }
}
