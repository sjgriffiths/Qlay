using System;
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
            double y = ((CircuitCanvas)Parent).QubitLineYValues.OrderBy(n => Math.Abs(top - n)).First();
            Canvas.SetTop(this, y - Height/2.0);
        }

        private void CircuitItem_Loaded(object sender, RoutedEventArgs e)
        {
            //Ensure correct style is assigned upon loading
            Style = FindResource("CircuitItemStyle") as Style;
        }
    }
}
