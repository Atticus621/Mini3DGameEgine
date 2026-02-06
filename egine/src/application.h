#pragma once


namespace engine {
	

	class Application {
	public:
		virtual bool Init() = 0;
		virtual void  Update(float delta) = 0;
		virtual void Destory() = 0;

		void SetNeedToBeClosed(bool flag);
		bool GetNeedToBeClosed()const;

	private:
		bool m_needToBeClosed = false;
	};
}