#include "Pass.h"

namespace rgph {

Pass::Pass(const std::string &passName) : _passName(passName) {

}

void Pass::setPassName(const std::string &passName) {
	_passName = passName;
}

const std::string & Pass::getPassName() const {
	return _passName;
}

void Pass::addPassResource(PassResourceBase *pResource) {
	_passResource.push_back(pResource);
}

}
