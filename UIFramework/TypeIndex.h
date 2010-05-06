#pragma once

namespace TypeIndex
{
    enum Value
    {
        Object,
            PropertyObject,
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
                Brush,
                    ImageBrush,
                    SolidColorBrush,
                Style,
                Setter,
            Collection,
                UIElementCollection,
                SetterCollection,
            Dictionary,
                ResourceDictionary,
            Float,
            String,
            RectF,
            ColorF,
            Visibility,
            Bool,
            RectangleEdge,
            BitmapSource,
            Geometry,
                RectangleGeometry,
                RoundedRectangleGeometry,
            RoutedEventArgs,
                InputEventArgs,
                    MouseEventArgs,
                        MouseButtonEventArgs
    };
}