#include <Dx12lib/Texture/DepthStencilTexture.h>
#include <Dx12lib/Resource/ResourceStateTracker.h>
#include <Dx12lib/Device/Device.h>


namespace dx12lib {

WRL::ComPtr<ID3D12Resource> DepthStencil2D::getD3DResource() const {
	return _pResource;
}

DepthStencil2D::~DepthStencil2D() {
	if (auto pSharedDevice = getDevice().lock()) {
		if (auto *pGlobalResourceState = pSharedDevice->getGlobalResourceState())
			pGlobalResourceState->removeGlobalResourceState(_pResource.Get());
	}
}

DepthStencil2D::DepthStencil2D(std::weak_ptr<Device> pDevice, 
	size_t width,
	size_t height,
	const D3D12_CLEAR_VALUE *pClearValue,
	DXGI_FORMAT depthStencilFormat)
{
	setDevice(pDevice);
	auto pSharedDevice = pDevice.lock();
	if (depthStencilFormat == DXGI_FORMAT_UNKNOWN)
		depthStencilFormat = pSharedDevice->getDesc().depthStencilFormat;

	if (pClearValue == nullptr)
		_clearValue.Format = depthStencilFormat;
	else
		_clearValue = *pClearValue;

	assert(_clearValue.Format != DXGI_FORMAT_UNKNOWN);
	pClearValue = &_clearValue;
	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = static_cast<UINT>(height);
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = depthStencilFormat;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	ThrowIfFailed(pDevice.lock()->getD3DDevice()->CreateCommittedResource(
		RVPtr(CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT)),
		D3D12_HEAP_FLAG_NONE,
		RVPtr(depthStencilDesc),
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		pClearValue,
		IID_PPV_ARGS(&_pResource)
	));

	pSharedDevice->getGlobalResourceState()->addGlobalResourceState(_pResource.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE);
}

DepthStencil2D::DepthStencil2D(std::weak_ptr<Device> pDevice, 
	WRL::ComPtr<ID3D12Resource> pResource, 
	D3D12_RESOURCE_STATES state, 
	const D3D12_CLEAR_VALUE *pClearValue)
{
	setDevice(pDevice);
	if (pClearValue != nullptr)
		_clearValue = *pClearValue;
	else
		_clearValue.Format = pResource->GetDesc().Format;

	assert(_clearValue.Format != DXGI_FORMAT_UNKNOWN);
	_pResource = pResource;
	auto pSharedDevice = pDevice.lock();
	pSharedDevice->getGlobalResourceState()->addGlobalResourceState(pResource.Get(), state);
}

}
