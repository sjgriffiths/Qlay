using System.ComponentModel;
using System.IO;
using System.Windows.Controls;
using System.Windows.Markup;

namespace QlayVisual
{
    /// <summary>
    /// Contains and organises the serialisable circuit data
    /// </summary>
    public class DataModel : INotifyPropertyChanged
    {
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


        public DataModel(ContentControl contentContainer)
        {
            ContentContainer = contentContainer;
        }

        public string Serialise()
        {
            return XamlWriter.Save(ContentContainer.Content);
        }

        public void Deserialise(Stream data)
        {
            ContentContainer.Content = XamlReader.Load(data) as CircuitCanvas;
        }

        public void New()
        {
            ContentContainer.Content = new CircuitCanvas();
            FilePath = null;
        }
    }
}
