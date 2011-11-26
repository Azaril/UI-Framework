#import <QuartzCore/QuartzCore.h>

#import "IPhoneTestAppViewController.h"
#import "EAGLView.h"
#include "EAGLContextBridgeGLES20.h"
#include "EAGLLayerStorageAllocator.h"

@interface IPhoneTestAppViewController ()
@property (nonatomic, assign) CADisplayLink *displayLink;
@property EAGLContextBridgeGLES20* contextBridge;
@property EAGLLayerStorageAllocator* storageAllocator;
@end

@implementation IPhoneTestAppViewController

@synthesize animating;
@synthesize displayLink;
@synthesize contextBridge;
@synthesize storageAllocator;

const wchar_t g_MarkupContent[] =
L"<Canvas>\n"
L"<Image Name=\"TestImage\" Source=\"testimg.png\" Height=\"100\" Canvas.Left=\"50\" Canvas.Top=\"250\"/>\n"
L"<Border Height=\"100\" Width=\"100\" Canvas.Left=\"50\" Canvas.Top=\"150\" BorderThickness=\"5\" BorderBrush=\"Red\"/>\n"
L"<Border Height=\"100\" Width=\"100\" Canvas.Left=\"100\" Canvas.Top=\"170\" BorderThickness=\"5\" BorderBrush=\"Green\"/>\n"
L"<Border Name=\"BlueBorder\" Height=\"100\" Width=\"100\" Canvas.Left=\"100\" Canvas.Top=\"100\">\n"
L"<Border.Background>\n"
//L"<SolidColorBrush Color=\"Red\"/>\n"
L"<LinearGradientBrush StartPoint=\"0,0\" EndPoint=\"1,1\">\n"
L"<GradientStop Color=\"Red\" Offset=\"0\" />\n"
L"<GradientStop Color=\"Blue\" Offset=\"0.25\" />\n"
L"<GradientStop Color=\"Green\" Offset=\"0.75\" />\n"
L"<GradientStop Color=\"Yellow\" Offset=\"1\" />\n"
L"</LinearGradientBrush>\n"
L"</Border.Background>\n"
L"<Border.Style><Style><Style.Triggers><EventTrigger RoutedEvent=\"UIElement.Loaded\">\n"
L"<BeginStoryboard>\n"
L"<Storyboard>\n"
//L"<FloatAnimation Storyboard.TargetName=\"BlueBorder\" Storyboard.TargetProperty=\"Canvas.Left\" Duration=\"5\" From=\"0\" To=\"200\" />\n"
L"</Storyboard>\n"
L"</BeginStoryboard>\n"
L"</EventTrigger></Style.Triggers></Style></Border.Style>\n"
L"</Border>\n"
L"<TextBlock Canvas.Left=\"25\" Canvas.Top=\"10\" Text=\"Test\" Foreground=\"White\"/>\n"
L"<TextBlock Canvas.Left=\"25\" Canvas.Top=\"50\" Text=\"Test\" >\n"
L"<TextBlock.Foreground>\n"
L"<LinearGradientBrush StartPoint=\"1,0\" EndPoint=\"1,1\">\n"
L"<GradientStop Color=\"Red\" Offset=\"0\" />\n"
L"<GradientStop Color=\"White\" Offset=\"0.5\" />\n"
L"<GradientStop Color=\"Green\" Offset=\"1\" />\n"
L"</LinearGradientBrush>\n"
L"</TextBlock.Foreground>\n"
L"</TextBlock>\n"
L"<Border Canvas.Left=\"150\" Canvas.Top=\"190\">\n"
L"<Border.Background>\n"
//L"<SolidColorBrush Color=\"Red\"/>\n"
L"<LinearGradientBrush StartPoint=\"0,0\" EndPoint=\"1,1\">\n"
L"<GradientStop Color=\"White\" Offset=\"0\" />\n"
L"<GradientStop Color=\"Black\" Offset=\"0.5\" />\n"
L"<GradientStop Color=\"White\" Offset=\"1\" />\n"
L"</LinearGradientBrush>\n"
L"</Border.Background>\n"
L"<TextBlock Text=\"Foo Bar\" Foreground=\"Red\"/>\n"
L"</Border>\n"
L"</Canvas>\n";

- (id)init
{
    self = [super init];
    
    if (self)
    {
        m_pClassResolver = NULL;
        m_pTypeConverter = NULL;
        m_pProviders = NULL;
        m_pParser = NULL;
        m_pUIHost = NULL;
        m_pFileResourceProvider = NULL;
        m_pBundleResourceProvider = NULL;
        m_pCompositeResourceProvider = NULL;
        m_pGraphicsDevice = NULL;
        m_pRenderTarget = NULL;
        m_pMouseController = NULL;
    }
    
    return self;
}

- (void)awakeFromNib
{
    HRESULT hr = S_OK;
    CTextProvider* pTextProvider = NULL;
    IReadStream* pContentStream = NULL;
    
    m_pClassResolver = NULL;
    m_pTypeConverter = NULL;
    m_pProviders = NULL;
    m_pParser = NULL;
    m_pUIHost = NULL;
    m_pFileResourceProvider = NULL;
    m_pBundleResourceProvider = NULL;
    m_pCompositeResourceProvider = NULL;
    m_pGraphicsDevice = NULL;
    m_pRenderTarget = NULL;
    m_pMouseController = NULL;    
    
    contextBridge = new EAGLContextBridgeGLES20();
    IFCPTR(contextBridge);
    
    storageAllocator = new EAGLLayerStorageAllocator(contextBridge, (CAEAGLLayer*)self.view.layer);
    IFCPTR(storageAllocator);
    
    animating = FALSE;
    animationFrameInterval = 1;
    self.displayLink = nil;
    
    IFC(CDynamicClassResolver::Create(&m_pClassResolver));
    
    IFC(CreateBasicTypeConverter(&m_pTypeConverter));
    
    IFC(CFileResourceProvider::Create(&m_pFileResourceProvider));
    
    IFC(CBundleFileResourceProvider::Create(&m_pBundleResourceProvider));
    
    IFC(CCompositeResourceProvider::Create(&m_pCompositeResourceProvider));
    
    IFC(m_pCompositeResourceProvider->AddResourceProvider(m_pBundleResourceProvider));
    IFC(m_pCompositeResourceProvider->AddResourceProvider(m_pFileResourceProvider));
    
    IFC(CProviders::Create(m_pClassResolver, m_pTypeConverter, m_pCompositeResourceProvider, &m_pProviders));
    
    IFC(CParser::Create(m_pProviders, &m_pParser));
    
    IFC(COpenGLES20GraphicsDevice::Create(contextBridge, &m_pGraphicsDevice));
    
    IFC(m_pGraphicsDevice->GetTextProvider(&pTextProvider));
    
    {
        const WCHAR fontFile[] = L"Opificio.ttf";
        
        IFC(pTextProvider->RegisterFont(m_pCompositeResourceProvider, fontFile, ARRAYSIZE(fontFile)));
    }
    
    IFC(m_pGraphicsDevice->CreateRenderTarget(storageAllocator, &m_pRenderTarget));
    
    {
        CFontDescription defaultFont(L"Opificio", 32, L"en-us");
        
        IFC(CUIHost::Create(m_pGraphicsDevice, m_pRenderTarget, m_pProviders, &defaultFont, &m_pUIHost));
    }
    
    IFC(m_pUIHost->GetMouseController(&m_pMouseController));
    
    {
        const WCHAR markupFile[] = L"testxml.xml";
        
        IFC(m_pBundleResourceProvider->ReadResource(markupFile, ARRAYSIZE(markupFile), &pContentStream));
    }                                                   
    
    IFC([self LoadContent:pContentStream]);
    
   [(EAGLView*)self.view setUIHost:m_pUIHost];
    
Cleanup:
    ReleaseObject(pTextProvider);
    ReleaseObject(pContentStream);
}

- (HRESULT)LoadContent:(IReadStream*)pContentStream
{
    HRESULT hr = S_OK;
    CObjectWithType* pParsedRootObject = NULL;
    CRootUIElement* pRootElement = NULL;
    CUIElement* pParsedRootElement = NULL;
    
    IFC(m_pParser->LoadFromStream(pContentStream, &pParsedRootObject));
    
    IFC(m_pUIHost->GetRootElement(&pRootElement));
    
    IFC(CastType(pParsedRootObject, &pParsedRootElement));
    
    IFC(pRootElement->SetChild(pParsedRootElement));
    
Cleanup:
    ReleaseObject(pRootElement);
    ReleaseObject(pParsedRootObject);
    
    return SUCCEEDED(hr);
}

- (void)dealloc
{
    ReleaseObject(m_pFileResourceProvider);
    ReleaseObject(m_pBundleResourceProvider);
    ReleaseObject(m_pCompositeResourceProvider);
    ReleaseObject(m_pClassResolver);
    ReleaseObject(m_pTypeConverter);
    ReleaseObject(m_pProviders);
    ReleaseObject(m_pParser);
    ReleaseObject(m_pRenderTarget);
    ReleaseObject(m_pGraphicsDevice);
    ReleaseObject(m_pMouseController);
    ReleaseObject(m_pUIHost); 
    
    if (contextBridge != NULL)
    {
        contextBridge->Release();
    }
    
    delete storageAllocator;
    
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
    [context release];
    
    [super dealloc];
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewWillAppear:(BOOL)animated
{
    [self startAnimation];
    
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [self stopAnimation];
    
    [super viewWillDisappear:animated];
}

- (void)viewDidUnload
{
	[super viewDidUnload];
	
    // Tear down context.
//    if ([EAGLContext currentContext] == context)
//        [EAGLContext setCurrentContext:nil];
//	self.context = nil;	
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    /*
	 Frame interval defines how many display frames must pass between each time the display link fires.
	 The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
	 */
    if (frameInterval >= 1) {
        animationFrameInterval = frameInterval;
        
        if (animating) {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation
{
    if (!animating) 
    {
        CADisplayLink *aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(drawFrame)];
        [aDisplayLink setFrameInterval:animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        
        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating) 
    {
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
}

- (void)drawFrame
{
    HRESULT hr = S_OK;
    
    if (m_pUIHost != NULL)
    {
        IFC(m_pUIHost->Render());
        
        contextBridge->Present(m_pRenderTarget->GetRenderBuffer());        
    }
    
Cleanup:
    ;
}

@end
