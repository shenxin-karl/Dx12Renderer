#pragma once
#include <optional>
#include <RenderGraph/RenderGraphStd.h>
#include <RenderGraph/Pass/Pass.h>
#include <Dx12lib/Context/ContextProxy.hpp>
#include <RenderGraph/Bindable/Bindable.hpp>
#include <RenderGraph/Pass/PassResourcePtr.hpp>
#include <Dx12lib/Texture/DepthStencilTexture.h>
#include <Dx12lib/Texture/RenderTargetTexture.h>

namespace rgph {

class ExecutablePass : public Pass {
public:
	explicit ExecutablePass(const std::string &passName, bool rtActivate = true, bool dsActivate = true);
	PassResourceBase *getPassResource(const std::string &resourceName) const final;
	void preExecute(dx12lib::DirectContextProxy pDirectCtx) override;
	void execute(dx12lib::DirectContextProxy pDirectCtx) override;
	void postExecute(dx12lib::DirectContextProxy pDirectCtx) override;
	void bindRenderTarget(dx12lib::IGraphicsContext &graphicsCtx);
	DXGI_FORMAT getRtFormat(size_t slot = 0) const;
	DXGI_FORMAT getDsFormat() const;
	void reset() override;
private:
	friend class PassResourceBase;
	friend class RenderGraph;
	void addPassResource(PassResourceBase *pResource);
	const std::vector<PassResourceBase *> &getPassResources() const;
private:
	std::vector<PassResourceBase *> _passResources;
public:
	std::optional<D3D12_RECT> customScissorRect;
	std::optional<D3D12_VIEWPORT> customViewport;
	size_t renderTargetMipmap = 0;
	PassResourcePtr<dx12lib::RenderTarget2D> pRenderTarget;
	PassResourcePtr<dx12lib::DepthStencil2D> pDepthStencil;
};

}
