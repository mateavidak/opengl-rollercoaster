# Serbian - Srpski
# Rolerkoster - 2D OpenGL Projekat

## 📋 Opis Projekta

2D grafička aplikacija rolerkostera, razvijena u C++ sa OpenGL 3+ bibliotekom. Projekat omogućava simulaciju vožnje rolerkostera sa interaktivnim upravljanjem putnicima i vozilom.

## 🎮 Funkcionalnosti

### Osnovna Mehanika

- **Vertikalna pruga** sa tri "brega" (uzbrdice i nizbrdice)
- **Vozilo rolerkostera** sa 8 sedišta
- **Sistem dodavanja putnika** - pritisak na Space dodaje putnika (prvo se popunjavaju prednja mesta)
- **Vezivanje sigurnosnih pojaseva** - levi klik miša na putnika stavlja teksturu pojasa
- **Simulacija vožnje** sa realističnom fizikom:
  - Ubrzano kretanje na početku
  - Konstantna brzina na ravnim površinama
  - Ubrzavanje na nizbrdicama
  - Usporavanje na uzbrdicama

### Sistem Stanja

- **Startovanje vožnje** - vozilo kreće samo ako su svi prisutni putnici vezani
- **Emergencija** - simulacija mučnine putnika:
  - Postepeno zaustavljanje vozila
  - Označavanje putnika zelenom bojom
  - Pauza od 10 sekundi
  - Vraćanje na početak malom konstantnom brzinom
- **Završetak vožnje** - automatsko odvezivanje putnika
- **Uklanjanje putnika** - klik na putnika da nestane
- **Nova tura** - dodavanje novih putnika nakon što svi izađu

### Korisnički Interfejs

- **Prilagođen kursor** sa izgledom šina
- **Full screen mod** sa 75 FPS frame limiterom
- **Escape** za izlazak iz aplikacije u bilo kom trenutku

## 🛠️ Tehnologije

- **Jezik**: C++
- **Grafička biblioteka**: OpenGL 3+ (programabilni pajplajn)
- **Biblioteke**: GLFW, GLAD, GLM, STB Image, FreeType

## 📦 Struktura Projekta

```
Rollercoaster/
├── Source/           # Izvorni C++ fajlovi
│   ├── Main.cpp
│   ├── Renderer.cpp
│   ├── Track.cpp
│   ├── Util.cpp
├── Shaders/          # Šejderi
│   ├── color.frag
│   ├── color.vert
│   ├── rect.frag
│   └── rect.vert
├── Header/           # Header fajlovi
├── Resources/        # Teksture i resursi
│   ├── cursor.png
│   ├── car.png
│   ├── person.png
│   ├── sick person.png
│   ├── belt.png
│   └── info.png
│   ├── background.png
├── Rollercoaster/
├── .gitignore
└── README.md
```

## 🚀 Pokretanje Projekta

### Koraci za Pokretanje

1. **Kloniranje repozitorijuma**:
   ```bash
   git clone https://github.com/mateavidak/Rollercoaster.git
   cd Rollercoaster
   ```

2. **Otvaranje projekta**:
   - Otvori `Rollercoaster.sln` u Visual Studio

## 🎯 Kontrole

| Taster/Akcija | Funkcija |
|---------------|----------|
| **Space** | Dodaj novog putnika u vozilo |
| **Enter** | Startuj vožnju (ako su svi putnici vezani) |
| **1-8** | Simuliraj da se putniku na tom mestu slošilo |
| **Levi klik miša** | Stavi sigurnosni pojas putniku / Ukloni putnika nakon vožnje |
| **Escape** | Izlaz iz aplikacije |

## 🎨 Implementirane OpenGL Tehnike

- ✅ Programabilni pajplajn (OpenGL 3+)
- ✅ Vertex i fragment šejderi
- ✅ Uniforme za transformacije i boje
- ✅ Providne boje i teksture (alpha blending)
- ✅ Teksturiranje (putnici, pojasevi, info)
- ✅ Prilagođen kursor sa teksturom
- ✅ Input handling (miš i tastatura)
- ✅ Frame limiting (75 FPS)
- ✅ Full screen mod
- ✅ Rendering krivih linija (Bezier krive)

## 👤 Autor

**Matea Vidak**   
Računarska grafika - 2D Projekat

---
# English - Engleski
# Rollercoaster - 2D OpenGL Project

## 📋 Project Description

A 2D graphics rollercoaster application developed in C++ with OpenGL 3+ library. The project enables simulation of a rollercoaster ride with interactive passenger and vehicle management.

## 🎮 Features

### Core Mechanics

- **Vertical track** with three "hills" (uphill and downhill sections)
- **Rollercoaster vehicle** with 8 seats
- **Passenger boarding system** - Space key adds passengers (front seats fill first)
- **Seatbelt fastening** - left mouse click on passenger applies seatbelt texture
- **Ride simulation** with realistic physics:
  - Accelerated movement at start
  - Constant speed on flat surfaces
  - Acceleration on downhill sections
  - Deceleration on uphill sections

### State System

- **Starting the ride** - vehicle starts only if all present passengers are buckled up
- **Emergency** - simulation of passenger sickness:
  - Gradual vehicle stop
  - Mark passenger with green color
  - 10-second pause
  - Return to start with slow constant speed
- **End of ride** - automatic unbuckling of passengers
- **Removing passengers** - click on passenger to remove
- **New round** - add new passengers after everyone exits

### User Interface

- **Custom cursor** with railroad track appearance
- **Full screen mode** with 75 FPS frame limiter
- **Escape** to exit application at any time

## 🛠️ Technologies

- **Language**: C++
- **Graphics Library**: OpenGL 3+ (programmable pipeline)
- **Libraries**: GLFW, GLAD, GLM, STB Image, FreeType

## 📦 Project Structure

```
Rollercoaster/
├── Source/           # C++ source files
│   ├── Main.cpp
│   ├── Renderer.cpp
│   ├── Track.cpp
│   ├── Util.cpp
├── Shaders/          # Shaders
│   ├── color.frag
│   ├── color.vert
│   ├── rect.frag
│   └── rect.vert
├── Header/           # Header files
├── Resources/        # Textures and resources
│   ├── cursor.png
│   ├── car.png
│   ├── person.png
│   ├── sick person.png
│   ├── belt.png
│   └── info.png
│   ├── background.png
├── Rollercoaster/
├── .gitignore
└── README.md
```

## 🚀 Running the Project

### Setup Steps

1. **Clone the repository**:
   ```bash
   git clone https://github.com/mateavidak/Rollercoaster.git
   cd Rollercoaster
   ```

2. **Open the project**:
   - Open `Rollercoaster.sln` in Visual Studio

## 🎯 Controls

| Key/Action | Function |
|------------|----------|
| **Space** | Add new passenger to vehicle |
| **Enter** | Start ride (if all passengers are buckled) |
| **1-8** | Simulate passenger sickness at that seat |
| **Left mouse click** | Apply seatbelt to passenger / Remove passenger after ride |
| **Escape** | Exit application |

## 🎨 Implemented OpenGL Techniques

- ✅ Programmable pipeline (OpenGL 3+)
- ✅ Vertex and fragment shaders
- ✅ Uniforms for transformations and colors
- ✅ Transparent colors and textures (alpha blending)
- ✅ Texturing (passengers, seatbelts, info)
- ✅ Custom cursor with texture
- ✅ Input handling (mouse and keyboard)
- ✅ Frame limiting (75 FPS)
- ✅ Full screen mode
- ✅ Curved line rendering (Bezier curves)

## 👤 Author

**Matea Vidak**  
Computer Graphics - 2D Project

---
