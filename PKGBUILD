# Contributor: Chris Down <christopher.down@iofc.org>
# Maintainer: Chris Down <christopher.down@iofc.org>

pkgname=cellout
pkgver=1.0
pkgrel=1
pkgdesc='Simple textual battery monitor.'
arch=('any')
url="https://github.com/cdown/${pkgname}"
license=('BSD')
source=("http://fakkelbrigade.eu/chris/aur/${pkgname}/${pkgname}-${pkgver}.tar.gz")
md5sums=('foobar')

build() {
	install -d "${pkgdir}/usr/bin/"
	install -m755 "${srcdir}/${pkgname}-${pkgver}/${pkgname}" "${pkgdir}/usr/bin/${pkgname}"
}
