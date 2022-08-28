#pragma once
#include <string>


namespace dx12lib {
	struct ICommonContext;
}

namespace rgph {

class ExecutablePass;
class PassResourceBase {
public:
	PassResourceBase(ExecutablePass *pExecutablePass, std::string resourceName, bool activate = true);
	void setFinished(bool bCond);
	bool isFinished() const;
	bool isActivated() const;
	void setResourceSource(const PassResourceBase *pResourceSource);
	const std::string &getResourceName() const;
	const PassResourceBase *getResourceSource() const;
	const ExecutablePass *getExecutablePass() const;
	virtual void link(dx12lib::ICommonContext &commonCtx) = 0;
	virtual void reset() = 0;
	virtual ~PassResourceBase() = default;
private:
	friend class RenderGraph;
	virtual bool tryLink() = 0;
protected:
	std::string _resourceName;
	ExecutablePass *_pExecutablePass;

	// 在编译 RenderGraph 时使用, 确定 Pass 执行的先后顺序
	bool _finished;
	bool _activated;
	const PassResourceBase *_pResourceSource;	
};

}
