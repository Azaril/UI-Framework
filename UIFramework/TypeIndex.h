#pragma once

namespace TypeIndex
{
    enum Value
    {
        Object,
            Collection,
                UIElementCollection,

            Float,
            String,
            RectF,
            ColorF,
            Visibility,

            Visual,
                UIElement,
                    FrameworkElement,
                        Panel,
                            StackPanel,
                            Canvas,
                            Grid,
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

            RoutedEventArgs,
                InputEventArgs,
                    MouseEventArgs,
                        MouseButtonEventArgs
    };
}