using System.Windows.Controls;
using System.Windows.Controls.Primitives;

namespace QlayVisual
{
    /// <summary>
    /// Draggable control item
    /// </summary>
    public class MoveThumb : Thumb
    {
        public MoveThumb()
        {
            DragDelta += new DragDeltaEventHandler(MoveThumb_DragDelta);
            DragCompleted += new DragCompletedEventHandler(MoveThumb_DragCompleted);
        }

        private void MoveThumb_DragDelta(object sender, DragDeltaEventArgs e)
        {
            //Update position by drag change event
            if (DataContext is CircuitItem ci)
            {
                double left = Canvas.GetLeft(ci);
                double top = Canvas.GetTop(ci);
                Canvas.SetLeft(ci, left + e.HorizontalChange);
                Canvas.SetTop(ci, top + e.VerticalChange);
            }
        }

        private void MoveThumb_DragCompleted(object sender, DragCompletedEventArgs e)
        {
            if (DataContext is CircuitItem ci)
            {
                CircuitCanvas cc = ci.Parent as CircuitCanvas;

                double left = Canvas.GetLeft(ci) + ci.Width/2.0;
                double top = Canvas.GetTop(ci) + ci.Height/2.0;
                double width = cc.ActualWidth;
                double height = cc.ActualHeight;

                //Delete self from canvas if dropped outside bounds
                if (left < 0 || left > width || top < 0 || top > height)
                    ci.DeleteFromCanvas();

                //Else, snap to nearest (currently only) qubit line
                else
                    ci.SnapToQubitLine();

                //Acknowledge change by clearing measurements
                cc.ClearMeasurementLabels();
            }
        }
    }
}
