using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Markup;

namespace QlayVisual
{
    /// <summary>
    /// Stored CircuitItem draggable from a toolbox onto the CircuitCanvas
    /// </summary>
    public class ToolboxItem : ContentControl
    {
        private Point? _dragStart = null;

        protected override void OnPreviewMouseDown(MouseButtonEventArgs e)
        {
            base.OnPreviewMouseDown(e);

            //Capture starting position of mouse drag
            _dragStart = new Point?(e.GetPosition(this));
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            //Reset _dragStart if no longer dragging
            if (e.LeftButton != MouseButtonState.Pressed)
                _dragStart = null;

            //Else, if we've started dragging
            if (_dragStart.HasValue)
            {
                //If a meaningful drag distance occurs, commence data transfer
                Vector change = _dragStart.Value - e.GetPosition(this);
                if (Math.Abs(change.X) > SystemParameters.MinimumHorizontalDragDistance ||
                    Math.Abs(change.Y) > SystemParameters.MinimumVerticalDragDistance)
                {
                    string xamlString = XamlWriter.Save(Content);
                    DataObject dataObject = new DataObject("CIRCUIT_ITEM", xamlString);
                    if (dataObject != null)
                        DragDrop.DoDragDrop(this, dataObject, DragDropEffects.Copy);
                }

                e.Handled = true;
            }
        }
    }
}
