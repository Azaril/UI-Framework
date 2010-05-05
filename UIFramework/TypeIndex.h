#pragma once

namespace TypeIndex
{
    enum Value
    {
        Object,
            Collection,
                UIElementCollection,

            Dictionary,
                ResourceDictionary,

            Float,
            String,
            RectF,
            ColorF,
            Visibility,
            Bool,

            RectangleEdge,

            Visual,
                UIElement,
                    FrameworkElement,
                        Panel,
                            StackPanel,
                            Canvas,
                            Grid,
                            DockPanel,
                        Image,
                        Decorator,
                            Border,
                        TextBlock,

            BitmapSource,

            Brush,
                ImageBrush,
                SolidColorBrush,

            Geometry,
                RectangleGeometry,
                RoundedRectangleGeometry,

            RoutedEventArgs,
                InputEventArgs,
                    MouseEventArgs,
                        MouseButtonEventArgs
    };
}