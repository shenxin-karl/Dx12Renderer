#pragma once
#include <string>
#include <RenderGraph/RenderGraphStd.h>
#include <Dx12lib/Context/ContextProxy.hpp>

namespace rg {

class PassResourceBase;
class Pass : public NonCopyable {
public:
	explicit Pass(const std::string &passName);
	virtual void execute(dx12lib::GraphicsContextProxy pGraphicsCtx) const = 0;
	virtual void reset() {}
	void setPassName(const std::string &passName);
	const std::string &getPassName() const;
private:
	friend class PassResourceBase;
	void addPassResource(PassResourceBase *pResource);
private:
	std::string _passName;
	std::vector<PassResourceBase *> _passResource;
};

}
