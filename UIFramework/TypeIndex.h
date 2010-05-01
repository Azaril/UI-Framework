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
                ImageBrush
    };
}