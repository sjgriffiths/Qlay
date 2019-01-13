using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Markup;
using System.Xml.Linq;

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
        public ContentControl ContentContainer { get; set; }

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
            get { return _filePath; }
            set
            {
                _filePath = value;
                OnPropertyChanged("FilePath");
                OnPropertyChanged("FileName");
                OnPropertyChanged("WindowTitle");
            }
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
                if (ci.FunctionName.StartsWith("M"))
                {
                    ci.Name = "M" + (mi++).ToString();
                    results.Add(ci.Name, Tuple.Create(0,0));
                }

            //Initialise Qlay QubitSystem
            Core.init();
            using (QubitSystem qs = new QubitSystem())
            {
                //List of n qubits, disposed of later when finished with
                List<Qubit> qubits = new List<Qubit>(CircuitCanvas.NumberOfQubits);
                try
                {
                    for (int i = 0; i < CircuitCanvas.NumberOfQubits; i++)
                        qubits.Add(new Qubit(qs));

                    //Run repeat simulations
                    for (int i = 0; i < repeats; i++)
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
                            args.Add(qubits[ci.QubitIndex]);
                            bool? result = (bool?)typeof(Gates).GetMethod(ci.FunctionName).Invoke(null, args.ToArray());

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

                        //To save time, reset and reuse the same system
                        qs.reset();
                    }
                }
                finally
                {
                    foreach (Qubit q in qubits)
                        if (q != null)
                            ((IDisposable)q).Dispose();
                }
            }

            return results;
        }


        /// <summary>
        /// Converts the given Dictionary into an XML string representation
        /// </summary>
        /// <typeparam name="TKey"></typeparam>
        /// <typeparam name="TValue"></typeparam>
        /// <param name="dict"></param>
        /// <returns></returns>
        public static string DictionaryToXML<TKey, TValue>(Dictionary<TKey, TValue> dict)
        {
            XElement xElement = new XElement("Dictionary",
                dict.Select(n => new XElement(n.Key.ToString(), n.Value)));

            return xElement.ToString();
        }

        /// <summary>
        /// Converts the given XML string into a Dictionary
        /// </summary>
        /// <param name="xml"></param>
        /// <returns></returns>
        public static Dictionary<string, string> XMLToDictionary(string xml)
        {
            return XElement.Parse(xml).Elements().ToDictionary(
                n => n.Name.ToString(), n => n.Value);
        }
    }
}
