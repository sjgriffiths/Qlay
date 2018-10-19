using System.Windows;
using System.Windows.Controls;

namespace QlayVisual
{
    /// <summary>
    /// Quantum logic circuit component
    /// </summary>
    class CircuitItem : ContentControl
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

        private void CircuitItem_Loaded(object sender, RoutedEventArgs e)
        {
            //Ensure correct style is assigned upon loading
            Style = FindResource("CircuitItemStyle") as Style;
        }
    }
}
