#pragma once
#include <string>
#include <RenderGraph/RenderGraphStd.h>
#include <Dx12lib/Context/ContextProxy.hpp>

namespace rgph {

enum class PassType {
	ExecutablePass,			// compute pass
	GraphicsPass,			// clear present pass
	RenderQueuePass,		// draw call
};

class PassResourceBase;
class Pass : public NonCopyable {
public:
	explicit Pass(const std::string &passName);
	virtual PassResourceBase *getPassResource(const std::string &resourceName) const { return nullptr; }
	virtual void preExecute(dx12lib::DirectContextProxy pDirectCtx) {}
	virtual void execute(dx12lib::DirectContextProxy pDirectCtx) {}
	virtual void postExecute(dx12lib::DirectContextProxy pDirectCtx) {}
	virtual void reset() {}
	void setPassName(const std::string &passName);
	const std::string &getPassName() const;
private:
	std::string _passName;
};

}
