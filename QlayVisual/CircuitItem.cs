using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;

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
        }

        /// <summary>
        /// Index of the qubit in the system this gate acts upon
        /// </summary>
        public int QubitIndex
        {
            get { return int.Parse(DataModel.XMLToDictionary((string)Tag)["QubitIndex"]); }
            set
            {
                Dictionary<string, string> dict = DataModel.XMLToDictionary((string)Tag);
                dict["QubitIndex"] = value.ToString();
                Tag = DataModel.DictionaryToXML(dict);
            }
        }

        /// <summary>
        /// Function name of this logic gate in the Qlay library
        /// </summary>
        public string FunctionName => DataModel.XMLToDictionary((string)Tag)["FunctionName"];

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
        }
    }
}
