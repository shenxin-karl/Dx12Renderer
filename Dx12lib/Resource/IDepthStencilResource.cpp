#include "IDepthStencilResource.h"

#include "Dx12lib/Device/Device.h"

namespace dx12lib {

IDepthStencil::IDepthStencil() {
	_clearValue.Format = DXGI_FORMAT_UNKNOWN;
	_clearValue.DepthStencil.Depth = 1.f;
	_clearValue.DepthStencil.Stencil = 0;
}

D3D12_CLEAR_VALUE IDepthStencil::getClearValue() const {
	return _clearValue;
}

/// ************************ IDepthStencil2D ********************************
#if 1
bool IDepthStencil2D::checkSRVState(D3D12_RESOURCE_STATES state) const {
	return state & D3D12_RESOURCE_STATE_DEPTH_READ;
}

const ShaderResourceView & IDepthStencil2D::getSRV(size_t mipSlice) const {
	assert(mipSlice != 0);
	return ITextureResource2D::getSRV(mipSlice);
}

const DepthStencilView & IDepthStencil2D::getDSV() const {
	if (_dsv.valid())
		return _dsv;

	auto pResource = getD3DResource();
	auto pSharedDevice = getDevice().lock();
	auto descriptor = pSharedDevice->allocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	_dsv = DepthStencilView(descriptor, this);
	pSharedDevice->getD3DDevice()->CreateDepthStencilView(
		pResource.Get(),
		nullptr,
		_dsv.getCPUDescriptorHandle()
	);

	return _dsv;
}
#endif

}
