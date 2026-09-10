# `<nazwa świata>`

> To jest szablon README projektu studenckiego. Repozytorium jest tworzone
> automatycznie w organizacji `lut-it-graphics-programming` pod nazwą
> `gp-{rok}-{login GitHub}-{numer indeksu}`. Po zaakceptowaniu zaproszenia
> zastąp pola `<...>`, usuń niepasujące warianty i aktualizuj dokument wraz
> z rozwojem świata. README ma pozwolić prowadzącemu zbudować, uruchomić
> i sprawdzić zgłoszony commit bez odgadywania konfiguracji.

## Świat

- Autor: `<imię i nazwisko albo identyfikator wymagany przez prowadzącego>`
- Biome: `<numer i nazwa biome'u>`
- Seed świata: `<seed używany przez aplikację>`
- Sposób wyznaczenia seeda, jeśli nie jest bezpośredni: `<opis albo nie dotyczy>`
- Reroll: `<nie / tak — pierwotny biome i zatwierdzona zmiana>`
- Krótki opis świata: `<2–4 zdania>`

## Karta semestralna

Jeśli realizujesz kartę semestralną, wybierz ją samodzielnie i wpisz tutaj najpóźniej do końca 6. tygodnia. Własna karta lub zmieniony zakres wymagają wcześniejszej akceptacji prowadzącego.

- Karta: `<nazwa / nie realizuję>`
- Poziom: `<1★ / 2★ / 3★ / nie dotyczy>`
- Status: `<wybrana / zaakceptowana / w trakcie / gotowa do obrony / nie realizuję>`
- Uzgodniona zmiana zakresu: `<opis i data akceptacji / nie dotyczy>`
- Finalny zakres: `<krótkie podsumowanie>`
- Przełącznik porównania z baseline'em: `<kontrolka lub klawisz>`
- Diagnostyka: `<tryb podglądu, pass, zasób lub licznik>`
- Pomiar, jeśli wymagany: `<metoda i wynik>`
- Dowody: `<ścieżki do obrazów, capture RenderDoc, nagranie lub opis miejsca w aplikacji>`

Karta jest opcjonalna i oceniana pass/fail dopiero podczas obrony końcowej. Nie zastępuje obowiązkowego rdzenia zadań.

## Wspierane środowisko

Oficjalny baseline:

- fizyczny komputer z Windows albo Linux,
- OpenGL 4.5 Core Profile,
- GLSL 450,
- CMake 3.21 lub nowszy i kompilator obsługujący C++20.

OpenGL 4.6 może być używane opcjonalnie, ale projekt musi zachować ścieżkę OpenGL 4.5/GLSL 450. macOS i maszyny wirtualne nie są wspierane; alternatywą jest komputer w laboratorium albo uzgodniony zdalny dostęp do fizycznej maszyny.

Konfiguracja użyta przez autora:

- System: `<Windows/Linux i wersja>`
- CPU: `<model>`
- GPU: `<model>`
- Sterownik: `<wersja>`
- CMake: `<wersja>`
- Kompilator: `<nazwa i wersja>`

## Diagnostyka przed pracą

Po pierwszym buildzie, przed rozpoczęciem implementacji, uruchom:

```text
<ścieżka-do-programu>/OpenGLGP --diagnostics
```

Najważniejsze wartości z raportu:

- `GL_VENDOR`: `<wartość>`
- `GL_RENDERER`: `<wartość>`
- `GL_VERSION`: `<wartość>`
- `GL_SHADING_LANGUAGE_VERSION`: `<wartość>`
- Profil i debug context: `<wartość>`
- Domyślny framebuffer sRGB: `<tak/nie>`
- Baseline OpenGL 4.5 Core/GLSL 450: `<spełniony/niespełniony>`

Nie rozpoczynaj pracy na środowisku, które nie potwierdza baseline'u. Szczegóły: [CI i oddawanie zadań](.assignments/CI_i_oddawanie.md).

## Struktura template'u

- `src/framework/` — dostarczona infrastruktura okna, pętli i diagnostyki;
- `src/project/` — punkt startowy kodu rozwijanego w zadaniach;
- `res/` — shadery, modele i tekstury świata;
- `tests/` — testy kodu CPU;
- `.assignments/` — treść i zasady zadań.

Kod dostarczony w `src/framework/` nie jest zaliczany do pakietu Modern OpenGL
studenta. Oceniane są rozwiązania dodane lub świadomie przebudowane w ramach
projektu. Strukturę `src/project/` można rozwijać i dzielić na kolejne moduły.

## Budowanie

Repozytorium studenckie powinno utrzymywać wersjonowane presety CMake dla wspieranej konfiguracji. Szablon udostępnia presety `dev`, `debug`, `release` i `ci`; jeśli zmienisz ich nazwy albo znaczenie, zaktualizuj ten rozdział. Nie commituj lokalnego `CMakeUserPresets.json`, katalogu `build` ani plików IDE.

Dostępne presety:

- codzienna praca: `dev`;
- jawny build Debug: `debug`;
- build Release: `release`;
- build i testy w CI: `ci`.

```text
git clone <adres-repozytorium>
cd <katalog-repozytorium>
cmake --preset dev
cmake --build --preset dev
```

Domyślna lokalizacja programu to `build/dev/src/Debug/OpenGLGP.exe` dla
wielokonfiguracyjnego generatora Visual Studio oraz `build/dev/src/OpenGLGP`
dla typowego jednokonfiguracyjnego generatora na Linuxie.

Jeżeli projekt wymaga dodatkowego kroku, opisz go tutaj: `<opis / brak>`.

## Uruchamianie

Tryb interaktywny:

```text
<ścieżka-do-programu>/OpenGLGP
```

Wymagane argumenty lub znane ograniczenia: `<opis / brak>`.

## Sterowanie

- Ruch kamery: `<klawisze>`
- Obrót kamery: `<mysz/klawisze>`
- Ruch góra/dół: `<klawisze>`
- Przechwycenie/zwolnienie kursora: `<klawisz>`
- Reset kamery: `<klawisz lub kontrolka>`
- Najważniejsze kontrolki ImGui: `<lista>`
- Przełączniki diagnostyczne: `<lista>`

## Hierarchia świata

Opisz zależności parent–child obiektu centralnego oraz grup sceny. Przykładowy format:

```text
World
├── Terrain
├── InstancedGroup
└── CentralObject
    ├── Body
    └── AnimatedPart
```

Sposób wyznaczania transformacji globalnej i elementy animowane: `<krótki opis>`.

## Potok renderowania

Kolejność passów:

1. `<pass>`
2. `<pass>`
3. `<pass>`

Obsługa HDR, tone mappingu i sRGB: `<wskaż, gdzie następuje dekodowanie, operacje liniowe i dokładnie jedna konwersja wyniku>`.

Sposób renderowania ImGui bez podwójnej konwersji sRGB: `<opis>`.

## Pomiary

Każdy wynik podaj razem z konfiguracją sprzętu, rozdzielczością, stanem VSync, parametrami sceny i metodą pomiaru.

- Wariant: `<np. 1 000 instancji>`
  - draw calle: `<liczba>`
  - czas CPU: `<wynik i metoda agregacji>`
  - czas GPU: `<wynik i metoda>`
- Wariant: `<np. 10 000 instancji>`
  - draw calle: `<liczba>`
  - czas CPU: `<wynik>`
  - czas GPU: `<wynik>`
- Wariant: `<np. 100 000 instancji>`
  - draw calle: `<liczba>`
  - czas CPU: `<wynik>`
  - czas GPU: `<wynik>`

Wnioski: `<krótka interpretacja wąskiego gardła i porównania>`.

Pomiary wydajności wykonuj na fizycznym GPU.

## Assety i licencje

Nie twórz osobnej bibliografii ani pliku `ASSETS.md`. Jeśli zewnętrzny asset ma plik licencji, pozostaw go obok assetu. Jeśli warunki wymagają oznaczenia autora, zachowaj tę informację w tym samym katalogu co asset.

Nietypowe informacje konieczne do poprawnego załadowania assetów: `<opis / brak>`.

## RenderDoc i materiały diagnostyczne

- Capture referencyjny: `<ścieżka lub sposób odtworzenia>`
- Draw call terenu: `<zdarzenie/marker>`
- Pass mapy cieni: `<zdarzenie/marker>`
- Instanced draw: `<zdarzenie/marker>`
- HDR attachment i final pass: `<zdarzenie/marker>`
- Debugowe etykiety zasobów/passów: `<opis>`

Pliki capture mogą być duże. Nie commituj ich, jeśli prowadzący nie wymaga tego wprost; podaj sposób odtworzenia klatki.

## Oddane wersje

- Zadanie 1: tag `zadanie-1`, SHA `<pełny SHA>`
- Zadanie 2: tag `zadanie-2`, SHA `<pełny SHA>`
- Zadanie 3: tag `zadanie-3`, SHA `<pełny SHA>`
- Zadanie 4: tag `zadanie-4`, SHA `<pełny SHA>`
- Poprawki: `<tag i SHA / brak>`

Zgłoszonego taga nie przesuwaj. Poprawki oznaczaj np. `zadanie-2-poprawa-1`. O terminowości decyduje kompletne zgłoszenie na zewnętrznej platformie przedmiotu.

## Dokumentacja przedmiotu

- [Opis kampanii](.assignments/README.md)
- [Zasady zaliczenia](.assignments/Zasady_zaliczenia.md)
- [Biomy](.assignments/Biomy.md)
- [Karty rozszerzeń](.assignments/Karty_rozszerzen.md)
- [CI i oddawanie zadań](.assignments/CI_i_oddawanie.md)
- [Zadanie 1 — Fundament świata](.assignments/Zadanie_1_Fundament_swiata.md)
- [Zadanie 2 — Żywy świat](.assignments/Zadanie_2_Zywy_swiat.md)
- [Zadanie 3 — Skala i światło](.assignments/Zadanie_3_Skala_i_swiatlo.md)
- [Zadanie 4 — Final Frame](.assignments/Zadanie_4_Final_Frame.md)
