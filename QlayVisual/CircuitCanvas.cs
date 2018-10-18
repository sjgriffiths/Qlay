using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;

namespace QlayVisual
{
    /// <summary>
    /// Canvas on which the circuit diagram can be constructed
    /// </summary>
    public class CircuitCanvas : Canvas
    {
        protected override void OnMouseLeftButtonUp(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonUp(e);

            //Test ellipse
            SolidColorBrush blue = new SolidColorBrush(Colors.Blue);
            Ellipse ellipse = new Ellipse
            {
                Fill = blue,
                IsHitTestVisible = false
            };

            //Assign to new CircuitItem
            CircuitItem ci = new CircuitItem
            {
                Content = ellipse,
                Width = 100,
                Height = 100
            };

            //Centre at cursor position
            SetLeft(ci, e.GetPosition(this).X - ci.Width/2.0);
            SetTop(ci, e.GetPosition(this).Y - ci.Height/2.0);

            Children.Add(ci);


            e.Handled = true;
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
    }
}
