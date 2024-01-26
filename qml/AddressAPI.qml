import QtQuick
import "qrc:/utils"

QtObject {
    id: root

    property CaraApi api: CaraApi {
        endpoint: "/address"
        onFailed: resp => errorOccoured(resp)
    }

    signal countriesFetched(countries: var)
    signal pronvicesFetched(provinces: var)
    signal municipalitiesFetched(municipalities: var)
    signal sectorsFetched(sectors: var)
    signal errorOccoured(resp: var)

    function fetchCountries() {
        api.get("/country/");
        api.succeeded.connect(resp => countriesFetched(resp.countries));
    }

    function fetchPronvinces(countryId: int) {
        api.get(`/country/${countryId}/`);
        api.succeeded.connect(resp => pronvicesFetched(resp.provinces));
    }

    function fetchMunicipalities(countryId: int, provinceId: int) {
        api.get(`/country/${countryId}/${provinceId}/`);
        api.succeeded.connect(resp => municipalitiesFetched(resp.municipalities));
    }

    function fetchSectors(countryId: int, provinceId: int, municipalityId: int) {
        api.get(`/country/${countryId}/${provinceId}/${municipalityId}/`);
        api.succeeded.connect(resp => sectorsFetched(resp.sectors));
    }
}
