#ifndef _DISORDER_ENGINE_H_
#define _DISORDER_ENGINE_H_


namespace Disorder
{
	class EngineStat
	{
	public:

		unsigned long FrameCounter;
		unsigned int  DrawTriNumber;

		void OnFrameBegin()
		{
			_lastFrameDrawTriNumber = DrawTriNumber;
			DrawTriNumber = 0;
		}

		void OnFrameEnd()
		{
		}

		EngineStat()
		{
			_bDrawEnable = true;	
			FrameCounter = 0;
			DrawTriNumber = 0;
			_lastFrameDrawTriNumber = 0;
		}

		void OnOff()
		{
			_bDrawEnable = !_bDrawEnable;
		}

		bool bDrawEnable()
		{
			return _bDrawEnable;
		}

		void DrawStat();

		void Tick(float deltaSeconds);

	private:
		bool _bDrawEnable;
		int _lastFrameDrawTriNumber;
		 
	};


	class Engine 
	{
	public:

		int GetMaxTickRate(float delta) { return 0;}
		void Tick(float deltaSeconds);

		Engine();
		~Engine();

	public :
	    EngineStat Stat;
		
	};

	

}



#endif