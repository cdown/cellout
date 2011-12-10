# Maintainer: Chris Down <iofc.org@christopher.down>

pkgname=cellout
pkgver=1.01
pkgrel=1
pkgdesc='A simple battery status printer.'
arch=('any')
url="https://github.com/cdown/${pkgname}"
license=('BSD')
source=("http://fakkelbrigade.eu/chris/software/${pkgname}/${pkgname}-${pkgver}.tar.gz")
md5sums=()

build() {
    gzip -9 "${srcdir}/${pkgname}.1"
    make
}

package() {
    make PREFIX="${pkgdir}/usr/bin/${pkgname}" install
    install -m755 \
        "${srcdir}/${pkgname}.1.gz" \
        "${pkgdir}/usr/share/man/man1/${pkgname}.1.gz"
}
