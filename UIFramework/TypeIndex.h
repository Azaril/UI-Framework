#pragma once

namespace TypeIndex
{
    enum Value
    {
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

        ImageSource
    };
}