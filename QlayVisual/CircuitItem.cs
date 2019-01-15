using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

using qlay.cli;

namespace QlayVisual
{
    /// <summary>
    /// Quantum logic circuit component
    /// </summary>
    public class CircuitItem : ContentControl
    {
        public CircuitItem()
        {
            Loaded += new RoutedEventHandler(CircuitItem_Loaded);
            MouseRightButtonDown += CircuitItem_MouseRightButtonDown;
        }

        //Runtime caches of tag dictionary properties
        private int? _qubitIndex;
        private string _functionName;
        private int? _orientation;

        /// <summary>
        /// Index of the qubit in the system this gate acts upon
        /// </summary>
        public int QubitIndex
        {
            get
            {
                if (!_qubitIndex.HasValue)
                    _qubitIndex = int.Parse(DataModel.XMLToDictionary((string)Tag)["QubitIndex"]);
                return _qubitIndex.Value;
            }
            private set
            {
                _qubitIndex = value;
                Dictionary<string, string> dict = DataModel.XMLToDictionary((string)Tag);
                dict["QubitIndex"] = value.ToString();
                Tag = DataModel.DictionaryToXML(dict);
            }
        }

        /// <summary>
        /// Function name of this logic gate in the Qlay library
        /// </summary>
        public string FunctionName
        {
            get
            {
                if (_functionName == null)
                    _functionName = DataModel.XMLToDictionary((string)Tag)["FunctionName"];
                return _functionName;
            }
        }

        /// <summary>
        /// For two-input gates, indicates whether the control index is +1 or -1
        /// </summary>
        public int Orientation
        {
            get
            {
                if (!_orientation.HasValue)
                    _orientation = int.Parse(DataModel.XMLToDictionary((string)Tag)["Orientation"]);
                return _orientation.Value;
            }
            private set
            {
                _orientation = value;
                Dictionary<string, string> dict = DataModel.XMLToDictionary((string)Tag);
                dict["Orientation"] = value.ToString();
                Tag = DataModel.DictionaryToXML(dict);
            }
        }

        /// <summary>
        /// The number of qubit inputs this gate function takes
        /// </summary>
        public int NumberOfQubitInputs => typeof(Gates).GetMethod(FunctionName).GetParameters()
            .Where(n => n.ParameterType == typeof(Qubit))
            .Count();

        /// <summary>
        /// Deletes this CircuitItem from the owning canvas
        /// </summary>
        public void DeleteFromCanvas()
        {
            ((CircuitCanvas)Parent).Children.Remove(this);
        }

        /// <summary>
        /// Snaps this CircuitItem into position on the nearest qubit line
        /// </summary>
        public void SnapToQubitLine()
        {
            double top = Canvas.GetTop(this) + Height/2.0;

            //Sort lines, along with their original indices, in distance from this gate
            var closestLine = ((CircuitCanvas)Parent).QubitLineYValues
                .Select((y, i) => new { y, i })
                .OrderBy(n => Math.Abs(top - n.y))
                .First();

            Canvas.SetTop(this, closestLine.y - Height/2.0);

            //Update qubit index as line's original index in list
            QubitIndex = closestLine.i;
        }

        private void CircuitItem_Loaded(object sender, RoutedEventArgs e)
        {
            //Ensure correct style is assigned upon loading
            Style = FindResource("CircuitItemStyle") as Style;

            //Set correct Y-axis orientation
            ((Canvas)Content).RenderTransformOrigin = new Point(0.5, 0.5);
            ((Canvas)Content).RenderTransform = new ScaleTransform(1, Orientation);
        }

        private void CircuitItem_MouseRightButtonDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            //Only applies to two-input gates
            if (NumberOfQubitInputs == 2)
            {
                //Flip orientation
                Orientation *= -1;
                ((Canvas)Content).RenderTransform = new ScaleTransform(1, Orientation);

                //Acknowledge change by clearing measurements
                ((CircuitCanvas)Parent).ClearMeasurementLabels();
            }
        }
    }
}
