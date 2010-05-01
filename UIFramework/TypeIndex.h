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
                SolidColorBrush
    };
}