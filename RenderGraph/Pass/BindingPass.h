#pragma once
#include <RenderGraph/RenderGraphStd.h>
#include <RenderGraph/Pass/Pass.h>
#include <Dx12lib/Context/ContextProxy.hpp>
#include <RenderGraph/Bindable/Bindable.hpp>
#include <RenderGraph/Pass/PassResourcePtr.hpp>
#include <Dx12lib/Texture/DepthStencilTexture.h>
#include <Dx12lib/Texture/RenderTargetTexture.h>

namespace rg {

class BindingPass : public Pass {
public:
	explicit BindingPass(const std::string &passName);
	virtual void link(dx12lib::ICommonContext &commonCtx) const;
	void addBind(std::shared_ptr<Bindable> pBindable);
	void bindAll(dx12lib::IGraphicsContext &graphicsCtx) const;
	void bindRenderTarget(dx12lib::IGraphicsContext &graphicsCtx) const;
	DXGI_FORMAT getRtFormat(size_t slot = 0) const;
	DXGI_FORMAT getDsFormat() const;
	void reset() override;
public:
	size_t renderTargetMipmap = 0;
	PassResourcePtr<dx12lib::RenderTarget2D> pRenderTarget;
	PassResourcePtr<dx12lib::DepthStencil2D> pDepthStencil;
private:
	std::vector<std::shared_ptr<Bindable>> _bindables;
};

}
