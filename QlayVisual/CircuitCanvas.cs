using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace QlayVisual
{
    /// <summary>
    /// Canvas on which the circuit diagram can be constructed
    /// </summary>
    public class CircuitCanvas : Canvas
    {
        protected override Size MeasureOverride(Size constraint)
        {
            Size size = new Size();
            foreach (UIElement e in Children)
            {
                double left = Canvas.GetLeft(e);
                double top = Canvas.GetTop(e);
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
