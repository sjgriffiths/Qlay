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

        private void CircuitItem_Loaded(object sender, RoutedEventArgs e)
        {
            //Ensure correct style is assigned upon loading
            Style = FindResource("CircuitItemStyle") as Style;
        }
    }
}
