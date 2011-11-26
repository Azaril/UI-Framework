#import "EAGLView.h"

#import <QuartzCore/QuartzCore.h>

#include "Point2F.h"

@interface EAGLView (PrivateMethods)
@end

@implementation EAGLView

+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

- (id)init
{
    self = [super init];
    
    if (self)
    {
        m_pUIHost = NULL;
        m_pMouseController = NULL;
    }
    
    return self;
}

- (id)initWithCoder:(NSCoder*)coder
{
    self = [super initWithCoder:coder];
    
	if (self) 
    {
        m_pUIHost = NULL;
        m_pMouseController = NULL;
        
        CAEAGLLayer* pEAGLLayer = (CAEAGLLayer *)self.layer;
        
        pEAGLLayer.opaque = TRUE;
        pEAGLLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], 
                                        kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, 
                                        kEAGLDrawablePropertyColorFormat,
                                        nil];
    }  
    
    return self;
}

- (void)dealloc
{
    ReleaseObject(m_pMouseController);
    ReleaseObject(m_pUIHost);
    
    [super dealloc];
}

- (void)layoutSubviews
{
    //TODO: Recreate frame buffer?
}

- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGPoint touchPoint = [[touches anyObject] locationInView:self];

    Point2F point(touchPoint.x, touchPoint.y);
    
    m_pMouseController->InjectMouseMove(point);
    
    m_pMouseController->InjectMouseButton(MouseButton::Left, MouseButtonState::Down);
}

- (void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    
}

- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGPoint touchPoint = [[touches anyObject] locationInView:self];
    
    Point2F point(touchPoint.x, touchPoint.y);
    
    m_pMouseController->InjectMouseMove(point);
    
    m_pMouseController->InjectMouseButton(MouseButton::Left, MouseButtonState::Up);
}

- (void) touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
}

- (void) setUIHost:(CUIHost*)pUIHost;
{
    ReleaseObject(m_pMouseController);
    
    ReplaceObject(m_pUIHost, pUIHost);
    
    m_pUIHost->GetMouseController(&m_pMouseController);
}

@end
