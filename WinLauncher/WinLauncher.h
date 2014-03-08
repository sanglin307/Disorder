#ifndef _DISORDER_WINLAUNCHER_H_
#define _DISORDER_WINLAUNCHER_H_

#include "..\Engine\EngineInclude.h"
#include "..\DX11Render\DX11RenderInclude.h"
#include "..\GLRender\GLRenderInclude.h"

namespace Disorder
{
   class WindowsViewport;
   typedef boost::shared_ptr<WindowsViewport> WindowsViewportPtr;
   class WinClient;
   typedef boost::shared_ptr<WinClient> WinClientPtr;

}

#include "WinClient.h"


#endif