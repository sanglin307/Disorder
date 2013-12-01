#ifndef _DISORDER_WINLAUNCHER_H_
#define _DISORDER_WINLAUNCHER_H_

#include "..\Engine\EngineInclude.h"
#include "..\DX11Render\DX11RenderInclude.h"

namespace Disorder
{
   class WindowsViewport;
   typedef boost::shared_ptr<WindowsViewport> WindowsViewportPtr;
   class WinFileSystem;
   typedef boost::shared_ptr<WinFileSystem> WinFileSystemPtr;
   class WinClient;
   typedef boost::shared_ptr<WinClient> WinClientPtr;

}

#include "WinClient.h"
#include "WinFileSystem.h"


#endif