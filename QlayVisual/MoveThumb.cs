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
        }

        private void MoveThumb_DragDelta(object sender, DragDeltaEventArgs e)
        {
            //Update position by drag change event
            if (DataContext is Control item)
            {
                double left = Canvas.GetLeft(item);
                double top = Canvas.GetTop(item);
                Canvas.SetLeft(item, left + e.HorizontalChange);
                Canvas.SetTop(item, top + e.VerticalChange);
            }
        }
    }
}
