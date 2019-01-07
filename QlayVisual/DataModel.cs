using System;
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

        /// <summary>
        /// References the current CircuitCanvas
        /// </summary>
        public CircuitCanvas CircuitCanvas => (CircuitCanvas) ContentContainer.Content;

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
            return XamlWriter.Save(CircuitCanvas);
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
        /// <param name="repeats"></param>
        public Dictionary<string, Tuple<int, int>> RunSimulation(int repeats)
        {
            //Consider logic gates in the order they appear, i.e. by X value
            var circuitItems = CircuitCanvas.Children.OfType<CircuitItem>().OrderBy(n => Canvas.GetLeft(n));

            //Collection of measurement results in (zeros, ones) pairs
            var results = new Dictionary<string, Tuple<int, int>>();

            //Name measurement items in order
            int mi = 0;
            foreach (CircuitItem ci in circuitItems)
                if (((string)ci.Tag).StartsWith("M"))
                {
                    ci.Name = "M" + (mi++).ToString();
                    results.Add(ci.Name, Tuple.Create(0,0));
                }

            //Run repeat simulations
            Core.init();
            for (int i = 0; i < repeats; i++)
            {
                using (QubitSystem qs = new QubitSystem())
                using (Qubit q = new Qubit(qs))
                {
                    foreach (CircuitItem ci in circuitItems)
                    {
                        //List of arguments to pass to logic gate
                        List<object> args = new List<object>();

                        //Obtain angle parameter, if one exists
                        foreach (UIElement uie in ((Canvas)ci.Content).Children)
                            if (uie is TextBox tb)
                                args.Add(Core.deg_to_rad(double.Parse(tb.Text)));

                        //Add qubit argument and call
                        args.Add(q);
                        bool? result = typeof(Gates).GetMethod((string)ci.Tag).Invoke(null, args.ToArray()) as bool?;

                        //Log measurement
                        if (result.HasValue)
                        {
                            Tuple<int, int> t = results[ci.Name];
                            if (result.Value)
                                results[ci.Name] = Tuple.Create(t.Item1, t.Item2 + 1);
                            else
                                results[ci.Name] = Tuple.Create(t.Item1 + 1, t.Item2);
                        }
                    }
                }
            }

            return results;
        }
    }
}
