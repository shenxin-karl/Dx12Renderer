#pragma once
#include <Dx12lib/dx12libStd.h>
#include <Dx12lib/Resource/ITextureResource.h>

namespace dx12lib {

interface IDepthStencil : virtual IResource {
	IDepthStencil();
	D3D12_CLEAR_VALUE getClearValue() const;
protected:
	D3D12_CLEAR_VALUE _clearValue;
};

interface IDepthStencil2D : IDepthStencil, ITextureResource2D {
	bool checkSRVState(D3D12_RESOURCE_STATES state) const override;
	const ShaderResourceView &getSRV(size_t mipSlice) const override;
	const DepthStencilView &getDSV() const;
private:
	mutable DepthStencilView _dsv;
};

}
