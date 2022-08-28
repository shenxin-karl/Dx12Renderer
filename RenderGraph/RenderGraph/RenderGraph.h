#pragma once
#include <memory>
#include <string>
#include <vector>
#include <Dx12lib/Context/ContextProxy.hpp>

namespace rgph {

class Pass;
class RenderGraph {
public:
	void addPass(std::shared_ptr<Pass> pPass);
	Pass *getPass(const std::string &passName) const;
	void execute(dx12lib::DirectContextProxy pDirectCtx);
	void reset();
	void finalize();
private:
	bool _finalized = false;
	std::vector<std::shared_ptr<Pass>> _passes;
	std::vector<std::shared_ptr<Pass>> _executeList;
};

}
