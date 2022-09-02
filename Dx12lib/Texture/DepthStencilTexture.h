#pragma once
#include <Dx12lib/dx12libStd.h>
#include <Dx12lib/Resource/IResource.h>
#include <Dx12lib/Descriptor/DescriptorAllocation.h>

namespace dx12lib {

class DepthStencil2D : public IDepthStencil2D {
public:
	WRL::ComPtr<ID3D12Resource> getD3DResource() const override;
	D3D12_CLEAR_VALUE getClearValue() const override;
	const ShaderResourceView & getSRV(size_t mipSlice = 0) const override;
	const DepthStencilView & getDSV() const override;
	~DepthStencil2D() override;
protected:
	DepthStencil2D(std::weak_ptr<Device> pDevice, 
		size_t width, 
		size_t height,
		const D3D12_CLEAR_VALUE *pClearValue = nullptr,
		DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_UNKNOWN
	);
	DepthStencil2D(std::weak_ptr<Device> pDevice, 
		WRL::ComPtr<ID3D12Resource> pResource,
		D3D12_RESOURCE_STATES state,
		const D3D12_CLEAR_VALUE *pClearValue = nullptr
	);
	void createViews(std::weak_ptr<Device> pDevice);
private:
	D3D12_CLEAR_VALUE _clearValue;
	std::weak_ptr<Device> _pDevice;
	DepthStencilView _depthStencilView;
	ShaderResourceView _shaderResourceView;
	WRL::ComPtr<ID3D12Resource> _pResource;
};

}
