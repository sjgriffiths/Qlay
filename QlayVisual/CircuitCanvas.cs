using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Xml;

namespace QlayVisual
{
    /// <summary>
    /// Canvas on which the circuit diagram can be constructed
    /// </summary>
    public class CircuitCanvas : Canvas
    {
        public CircuitCanvas()
        {
            Loaded += new RoutedEventHandler(CircuitCanvas_Loaded);
        }

        /// <summary>
        /// Vertical separation between qubit lines
        /// </summary>
        public readonly double LINE_SEPARATION = 90;

        /// <summary>
        /// Number of qubits currently in the circuit
        /// </summary>
        public int NumberOfQubits => int.Parse(DataModel.XMLToDictionary((string)Tag)["NumberOfQubits"]);

        /// <summary>
        /// A list of the Y value of each qubit line
        /// </summary>
        public List<double> QubitLineYValues
        {
            get
            {
                var li = new List<double>();
                foreach (Line l in Children.OfType<Line>())
                    li.Add(l.Y1);
                return li;
            }
        }

        /// <summary>
        /// Redraws n qubit lines
        /// </summary>
        /// <param name="n"></param>
        public void SetQubitLines(int n)
        {
            //Delete preexisting qubit lines
            foreach (Line l in Children.OfType<Line>().ToList())
                Children.Remove(l);

            //Add n qubit lines
            for (int i = 0; i < n; i++)
            {
                Line l = new Line()
                {
                    Stroke = new SolidColorBrush(Colors.Black),
                    StrokeThickness = 4,
                    X1 = 0,
                    Y1 = LINE_SEPARATION * (i + 1),
                    X2 = Width,
                    Y2 = LINE_SEPARATION * (i + 1),
                };

                Children.Add(l);
                SetZIndex(l, -1);
            }
        }

        /// <summary>
        /// Updates all measurement labels with the given dictionary of results
        /// </summary>
        /// <param name="data"></param>
        public void SetMeasurementLabels(Dictionary<string, Tuple<int, int>> data)
        {
            foreach (CircuitItem ci in Children.OfType<CircuitItem>())
                if (ci.Name.StartsWith("M"))
                    foreach (Label l in ((Canvas)ci.Content).Children.OfType<Label>())
                    {
                        var t = data[ci.Name];
                        l.Content = t.Item1 + " , " + t.Item2;
                    }
        }

        /// <summary>
        /// Clears all measurement labels
        /// </summary>
        public void ClearMeasurementLabels()
        {
            foreach (CircuitItem ci in Children.OfType<CircuitItem>())
                if (ci.Name.StartsWith("M"))
                    foreach (Label l in ((Canvas)ci.Content).Children.OfType<Label>())
                        l.Content = "";
        }

        /// <summary>
        /// Changes the number of qubits in the circuit by the given amount
        /// </summary>
        public void ChangeQubits(int change)
        {
            //Get info from tag dictionary and update
            Dictionary<string, string> dict = DataModel.XMLToDictionary((string)Tag);
            int numberOfQubits = int.Parse(dict["NumberOfQubits"]) + change;

            if (numberOfQubits > 0)
            {
                //Update canvas
                SetQubitLines(numberOfQubits);

                //Update tag dictionary
                dict["NumberOfQubits"] = numberOfQubits.ToString();
                Tag = DataModel.DictionaryToXML(dict);
            }
        }

        private void CircuitCanvas_Loaded(object sender, RoutedEventArgs e)
        {
            //Ensure correct style is assigned upon loading
            Style = FindResource("CircuitCanvasStyle") as Style;

            //Initialise tag dictionary if the canvas is new
            if (Tag == null)
                Tag = DataModel.DictionaryToXML(new Dictionary<string, string>()
                {
                    { "NumberOfQubits", "1" }
                });

            //Initialise qubit lines
            SetQubitLines(int.Parse(DataModel.XMLToDictionary((string)Tag)["NumberOfQubits"]));
        }

        protected override Size MeasureOverride(Size constraint)
        {
            //Resize canvas when instructed to
            Size size = new Size();
            foreach (UIElement e in Children)
            {
                double left = GetLeft(e);
                double top = GetTop(e);
                left = double.IsNaN(left) ? 0 : left;
                top = double.IsNaN(top) ? 0 : top;

                e.Measure(constraint);

                Size desired = e.DesiredSize;
                if (!double.IsNaN(desired.Width) && !double.IsNaN(desired.Height))
                {
                    size.Width = Math.Max(size.Width, left + desired.Width);
                    size.Height = Math.Max(size.Height, top + desired.Height);
                }
            }

            size.Width += 10;
            size.Height += 10;

            return size;
        }

        protected override void OnDrop(DragEventArgs e)
        {
            base.OnDrop(e);

            //Acknowledge change by clearing measurements
            ClearMeasurementLabels();

            //Receive data from ToolboxItem
            string xamlString = e.Data.GetData("CIRCUIT_ITEM") as string;
            if (!string.IsNullOrEmpty(xamlString))
            {
                FrameworkElement content = null;

                using (StringReader sr = new StringReader(xamlString))
                using (XmlReader xr = XmlReader.Create(sr))
                {
                    content = XamlReader.Load(xr) as FrameworkElement;
                }

                if (content != null)
                {
                    //Disable hit test for all children except textboxes, which should now be visible
                    foreach (UIElement tb in ((Canvas)content).Children)
                    {
                        if (tb is TextBox)
                            tb.Visibility = Visibility.Visible;
                        else
                            tb.IsHitTestVisible = false;
                    }

                    //Copy content into CircuitItem
                    CircuitItem ci = new CircuitItem
                    {
                        Content = content,
                        Width = content.Width,
                        Height = content.Height,

                        //Initialise tag dictionary with gate function name
                        Tag = DataModel.DictionaryToXML(new Dictionary<string, string>()
                        {
                            { "FunctionName" , (string)content.Tag }
                        })
                    };
                    Children.Add(ci);

                    //Set to dropped position
                    SetLeft(ci, e.GetPosition(this).X - ci.Width/2.0);
                    SetTop(ci, e.GetPosition(this).Y - ci.Height/2.0);

                    //Snap to nearest qubit line
                    ci.SnapToQubitLine();
                }

                e.Handled = true;
            }
        }
    }
}
