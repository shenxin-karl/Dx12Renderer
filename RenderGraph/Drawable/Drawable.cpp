#include "Drawable.h"
#include <RenderGraph/Technique/Technique.h>

#include "Dx12lib/Buffer/IndexBuffer.h"
#include "Dx12lib/Buffer/VertexBuffer.h"

namespace rgph {

void Drawable::submit(const TechniqueFlag &techniqueFlag) const {
	assert(_pGeometry != nullptr);
	assert(_pTransformCBuf != nullptr);
	for (auto &pTechnique : _techniques) {
		pTechnique->submit(
			techniqueFlag,
			_pGeometry.get(),
			_pTransformCBuf.get()
		);
	}
}

bool Drawable::addTechnique(std::shared_ptr<Technique> pTechnique) {
	assert(pTechnique);
	if (hasTechnique(pTechnique->getTechniqueType()))
		return false;

	_techniqueFlag.set(pTechnique->getTechniqueType());
	_techniques.push_back(std::move(pTechnique));
	return true;
}

bool Drawable::hasTechnique(const TechniqueType &techniqueType) const {
	return _techniqueFlag.test(techniqueType);
}

bool Drawable::removeTechnique(const TechniqueType &techniqueType) {
	if (!hasTechnique(techniqueType))
		return false;

	for (auto iter = _techniques.begin(); iter != _techniques.end(); ++iter) {
		if ((*iter)->getTechniqueType() & techniqueType) {
			_techniques.erase(iter);
			return true;
		}
	}

	assert(false);
	return false;
}

void Drawable::setTechniqueActive(const TechniqueType &techniqueType, bool bActivate) {
	if (!hasTechnique(techniqueType))
		return;

	for (auto iter = _techniques.begin(); iter != _techniques.end(); ++iter) {
		if ((*iter)->getTechniqueType() & techniqueType) {
			(*iter)->setActive(bActivate);
			return;
		}
	}
}

void Drawable::clearTechnique() {
	_techniques.clear();
	_techniqueFlag.reset();
}

std::shared_ptr<Geometry> Drawable::getGeometry() const {
	return _pGeometry;
}

std::shared_ptr<TransformCBuffer> Drawable::getTransformCBuffer() const {
	return _pTransformCBuf;
}

void Drawable::setGeometry(std::shared_ptr<Geometry> pGeometry) {
	assert(pGeometry != nullptr);
	_pGeometry = std::move(pGeometry);
}

void Drawable::setGeometry(std::shared_ptr<TransformCBuffer> pTransformCBuf) {
	assert(pTransformCBuf != nullptr);
	_pTransformCBuf = std::move(pTransformCBuf);
}

}
