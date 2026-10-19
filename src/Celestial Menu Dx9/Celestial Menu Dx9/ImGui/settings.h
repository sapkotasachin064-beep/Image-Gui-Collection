
inline int font_item = 2;
inline float rectsize = 0;
inline bool MultiColor = false;
inline bool barstyle = false;
inline bool text_toupper = true;


bool boxon = false;
bool Healthbar = false;
bool Armorbar = false;
bool Reloadbar = false;
bool nickname = false;
bool dist = false;
bool flash = false;
bool blind = false;
bool lc = false;
bool money = false;
bool scope = false;
bool ping = false;
bool kd = false;
bool weaponicon = false;

float boxrounding = 0.f;
float lineLength = 10.0f;
bool Corneredbox = false;




static float b0x[4] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 255 / 255.f };

static float boneThickness = 1.0f;

static float playerbone[4] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 255 / 255.f };
static float permissionsbone[4] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 255 / 255.f };


static bool esp_ff[14] = {
true, true, true, true, true, true, true, true, true, true, true, true, true, true
};


const char* esp_ff_english[] = {
    "Gradient Health", "2D Box", "Bone", "Name", "Kills",
    "Team", "Rank", "Weapon Icon", "Distance",
    "Spectator", "KDA", "Level"
};

