#include "SystemManager.h"
#include "RAMRecipes.h"
#include "ScreenMessages.h"
#include "smath.h"

#define BUTTON_HELD_EVENT_SPEED 1000

//HOLD Stages
#define STAGE1_VALUE 1
#define STAGE1_TIME_MS 6000

#define STAGE2_VALUE 5
#define STAGE2_TIME_MS 11000

#define STAGE3_VALUE 10


#define MAX_ON_TIME_S_PER_PULSE 255
#define MAX_OFF_TIME_S_PER_PULSE 255
#define MIN_FIRST_PULSE_ON_TIME_S 1         //Changed by FJ was 30
#define MIN_PREV_OFF_TIME_FOR_NEW_PULSE 10

typedef enum COFFEE_BEVERAGE_EDITOR
    {
    COFFEE_BEVERERAGE_EDITOR_INIT,
    COFFEE_BEVERAGE_SELECT_RECIPE, //FJ added        
    COFFEE_BEVERAGE_SELECT_HEAD,
    COFFEE_BEVERAGE_SELECT_EDIT_ENTIRE_RECIPE_OR_SPECIFIC_PULSE,
    COFFEE_BEVERAGE_SELECT_PULSE,
    COFFEE_BEVERAGE_EDIT_SELECTED_PULSE_ON,
    COFFEE_BEVERAGE_EDIT_SELECTED_PULSE_OFF,

    } CoffeeBeverageEditorStates_t;

CoffeeBeverageEditorStates_t currentCoffeeBeverageEditState = COFFEE_BEVERERAGE_EDITOR_INIT;
CoffeeBeverage editedRecipe;
uint8_t pulseNumberEdited = 0;
bool singleHeadUnit = false;
bool recipeSelectorStateInitialized = false;
int buttonHoldCount = 0;
signed int lastTouchButton;
int eventCounter = 0;

uint8_t headSelectIndex = 0;
extern int numberOfHeads;
static int currentSingleHeadRecipeMenuItems = 2;
static int currentDoubleHeadRecipeMenuItems = 5;
int8_t recipeSelectIndex = 0; //FJ Changed to signed integer
int8_t recipeRightSelectIndex = 3; //FJ added on 4/20 because RIGHT_SIDE_RECIPE_INDEX = 3
bool recipeSelectorInitialized = false;
char recipeSelectorBottomLine[20];
ScreenMessages RecipeSelectorMsg("RECIPE SELECTED", recipeSelectorBottomLine, DEFAULT_STATIC_MENU_PRINT_TIME);

void updateRecipeSelectMsg() //FJ added for Add Recipes project
    {
    if (headSelectIndex == LEFT_SIDE_RECIPE_INDEX)
        {
        if (recipeSelectIndex == RECIPE_ONE_INDEX)
            {
            sprintf(recipeSelectorBottomLine, "%s", "1");
            }
        else if (recipeSelectIndex == RECIPE_TWO_INDEX)
            {
            sprintf(recipeSelectorBottomLine, "%s", "2");
            }
        else if (recipeSelectIndex == RECIPE_THREE_INDEX)
            {
            sprintf(recipeSelectorBottomLine, "%s", "3");
            }

        }
    else
        {
        if (recipeRightSelectIndex == RECIPE_FOUR_INDEX)
            {
            sprintf(recipeSelectorBottomLine, "%s", "4");
            }
        else if (recipeRightSelectIndex == RECIPE_FIVE_INDEX)
            {
            sprintf(recipeSelectorBottomLine, "%s", "5");
            }
        else if (recipeRightSelectIndex == RECIPE_SIX_INDEX)
            {
            sprintf(recipeSelectorBottomLine, "%s", "6");
            }
        }


    }

void SystemManager::recipeSelectHandler() //FJ added to SytemManager.h System class under void headSelectHandler();
    {
    if (!recipeSelectorInitialized)
        {
        updateRecipeSelectMsg();
        myUI->Screen->showMessageNow(&RecipeSelectorMsg);
        recipeSelectorInitialized = true;
        }

    if (releasedTouchValue == TOUCH_NEXT) //FJ changed
        {
        if (headSelectIndex == LEFT_SIDE_RECIPE_INDEX)
            {
            recipeSelectIndex++;
            if (recipeSelectIndex > currentSingleHeadRecipeMenuItems)
                {
                recipeSelectIndex = RECIPE_ONE_INDEX;
                }
            updateRecipeSelectMsg();
            myUI->Screen->showMessageNow(&RecipeSelectorMsg);
            }
        else if (headSelectIndex == RIGHT_SIDE_RECIPE_INDEX)
            {
            recipeRightSelectIndex++;
            if (recipeRightSelectIndex > currentDoubleHeadRecipeMenuItems)
                {
                recipeRightSelectIndex = RECIPE_THREE_INDEX;
                }
            updateRecipeSelectMsg();
            myUI->Screen->showMessageNow(&RecipeSelectorMsg);
            }

        }

    else if (releasedTouchValue == TOUCH_PREVIOUS) //FJ changed
        {
        if (headSelectIndex == LEFT_SIDE_RECIPE_INDEX)
            {
            recipeSelectIndex--;
            if (recipeSelectIndex < 0)
                {
                recipeSelectIndex = RECIPE_THREE_INDEX;
                }
            updateRecipeSelectMsg();
            myUI->Screen->showMessageNow(&RecipeSelectorMsg);
            }
        else if (headSelectIndex == RIGHT_SIDE_RECIPE_INDEX)
            {
            recipeRightSelectIndex--;
            if (recipeRightSelectIndex < 3)
                {
                recipeRightSelectIndex = RECIPE_SIX_INDEX;
                }
            updateRecipeSelectMsg();
            myUI->Screen->showMessageNow(&RecipeSelectorMsg);
            }

        }

    else if (releasedTouchValue == TOUCH_ACCEPT)
        {
        if (headSelectIndex == LEFT_SIDE_RECIPE_INDEX)
            {
            editedRecipe = *getCoffeeBeveragePointer(recipeSelectIndex);
            }
        else
            {
            editedRecipe = *getCoffeeBeveragePointer(recipeRightSelectIndex);
            }
        currentCoffeeBeverageEditState = COFFEE_BEVERAGE_SELECT_EDIT_ENTIRE_RECIPE_OR_SPECIFIC_PULSE;
        recipeSelectorInitialized = false;
        return;
        }
    else if (releasedTouchValue == TOUCH_CANCEL)
        {
        recipeSelectorInitialized = false;
        currentCoffeeBeverageEditState = COFFEE_BEVERERAGE_EDITOR_INIT;
        changeState(MENU_BROWSE);
        return;
        }
    }
// End of Recipe Selector


bool headSelectorInitialized = false;
char headSelectorBottomLine[20];
ScreenMessages HeadSelectorMsg("HEAD SELECTED", headSelectorBottomLine, DEFAULT_STATIC_MENU_PRINT_TIME);

void updateHeadSelectMsg()
    {
    if (headSelectIndex == LEFT_SIDE_RECIPE_INDEX)
        {
        sprintf(headSelectorBottomLine, "%s", "LEFT SIDE");
        }
    else
        {
        sprintf(headSelectorBottomLine, "%s", "RIGHT SIDE");
        }
    }

void SystemManager::headSelectHandler()
    {
    if (!headSelectorInitialized)
        {
        updateHeadSelectMsg();
        myUI->Screen->showMessageNow(&HeadSelectorMsg);
        headSelectorInitialized = true;
        }

    if (releasedTouchValue == TOUCH_NEXT && headSelectIndex == LEFT_SIDE_RECIPE_INDEX) //FJ changed separated TOUCHES
        {
        headSelectIndex = headSelectIndex + 3;
        updateHeadSelectMsg();
        myUI->Screen->showMessageNow(&HeadSelectorMsg);
        }
    else if (releasedTouchValue == TOUCH_PREVIOUS && headSelectIndex == RIGHT_SIDE_RECIPE_INDEX)
        {
        headSelectIndex = headSelectIndex - 3;
        updateHeadSelectMsg();
        myUI->Screen->showMessageNow(&HeadSelectorMsg);
        }

    else if (releasedTouchValue == TOUCH_ACCEPT)
        {
        editedRecipe = *getCoffeeBeveragePointer(headSelectIndex);
        currentCoffeeBeverageEditState = COFFEE_BEVERAGE_SELECT_RECIPE; //FJ changed from COFFEE_BEVERERAGE_EDITOR_INIT to COFFEE_BEVERAGE_SELECT_RECIPE
        headSelectorInitialized = false;
        return;
        }
    else if (releasedTouchValue == TOUCH_CANCEL)
        {
        headSelectorInitialized = false;
        currentCoffeeBeverageEditState = COFFEE_BEVERERAGE_EDITOR_INIT;
        changeState(MENU_BROWSE);
        return;
        }
    }
// end of select head

bool editEntireRecipeHandlerInitialized = false;
char editEntireRecipeBottomLine[20];
bool editEntireRecipeAnswer = false;
ScreenMessages EditEntireRecipeMsg("EDIT FULL RECIPE", editEntireRecipeBottomLine, DEFAULT_STATIC_MENU_PRINT_TIME);

void updateEntireRecipeMsg(void)
    {
    if (editEntireRecipeAnswer)
        {
        sprintf(editEntireRecipeBottomLine, "YES");
        }
    else
        {
        sprintf(editEntireRecipeBottomLine, "NO");
        }
    }

void SystemManager::editEntireRecipeHandler(void)
    {
    if (!editEntireRecipeHandlerInitialized)
        {
        updateEntireRecipeMsg();
        myUI->Screen->showMessageNow(&EditEntireRecipeMsg);
        editEntireRecipeAnswer = false;
        editEntireRecipeHandlerInitialized = true;
        }

    if (releasedTouchValue == TOUCH_NEXT || releasedTouchValue == TOUCH_PREVIOUS)
        {
        editEntireRecipeAnswer = !editEntireRecipeAnswer;
        updateEntireRecipeMsg();
        myUI->Screen->showMessageNow(&EditEntireRecipeMsg);
        }
    else if (releasedTouchValue == TOUCH_ACCEPT)
        {
        if (editEntireRecipeAnswer)
            {
            pulseNumberEdited = 0;
            currentCoffeeBeverageEditState = COFFEE_BEVERAGE_EDIT_SELECTED_PULSE_ON;
            }
        else
            {
            currentCoffeeBeverageEditState = COFFEE_BEVERAGE_SELECT_PULSE;
            }
        editEntireRecipeHandlerInitialized = false;
        return;
        }
    else if (releasedTouchValue == TOUCH_CANCEL)
        {
        editEntireRecipeHandlerInitialized = false;
        if (machineFeature->numberOfHeads == 1)
            {
            currentCoffeeBeverageEditState = COFFEE_BEVERAGE_SELECT_RECIPE; //FJ changed from [COFFEE_BEVERERAGE_EDITOR_INIT;]
            //FJ Deleted [changeState(MENU_BROWSE);]
            }
        else
            {
            currentCoffeeBeverageEditState = COFFEE_BEVERAGE_SELECT_HEAD;
            }
        return;
        }
    }
//End of Entire Recipe Handler

bool pulseSelectInitialized = false;
char pulseSelectBottomLine[20];
ScreenMessages PulseSelectMsg("PULSE SELECTED", pulseSelectBottomLine, DEFAULT_STATIC_MENU_PRINT_TIME);

void updatePulseSelectMsg()
    {
    if (pulseNumberEdited == 0)
        {
        sprintf(pulseSelectBottomLine, "PRE-INFUSION");
        }
    else
        {
        sprintf(pulseSelectBottomLine, "PULSE #%d", pulseNumberEdited);
        }
    }

void checkMinimumOffTimeOnPreviousPulse(void)
    {
    if (pulseNumberEdited != 0)
        {
        if (editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited - 1] < MIN_PREV_OFF_TIME_FOR_NEW_PULSE && editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] != 0)
            {
            editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited - 1] = MIN_PREV_OFF_TIME_FOR_NEW_PULSE;
            }
        }
    }

void recipeCleanup(void)
    {
    //Clear out pulse on and pulse off times when a intermediate pulse is set to zero
    if (editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] == 0 && pulseNumberEdited != 0)
        {
        //Zero out previous pulse's off time 
        editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited - 1] = 0;

        //Zero out 
        for (int i = pulseNumberEdited; i < NUMBER_OF_PULSES; i++)
            {
            editedRecipe.size[0].pulseOnTimeSeconds[i] = 0;
            editedRecipe.size[0].pulseOffTimeSeconds[i] = 0;
            }
        editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited - 1] = 0;
        }

    //Make sure all pulses up to edited pulse have a minimum off time
    if (pulseNumberEdited != 0)
        {
        for (int i = 0; i < pulseNumberEdited - 1; i++)
            {
            if (editedRecipe.size[0].pulseOffTimeSeconds[i] < MIN_PREV_OFF_TIME_FOR_NEW_PULSE)
                {
                editedRecipe.size[0].pulseOffTimeSeconds[i] = MIN_PREV_OFF_TIME_FOR_NEW_PULSE;
                }
            }
        }
    }

int getNumberOfPulses(void)
    {
    int lastestPulse = 1;
    for (int i = 0; i < NUMBER_OF_PULSES; i++)
        {
        if (editedRecipe.size[0].pulseOnTimeSeconds[i] == 0)
            {
            break;
            }
        lastestPulse = i + 1;
        }
    return lastestPulse;
    }

void findLastOnTimePulseClearSecondLastOffPulse(void)
    {
    int pulses = getNumberOfPulses();
    if (pulses != 1)
        {
        editedRecipe.size[0].pulseOffTimeSeconds[pulses - 1] = 0;
        }
    else
        {
        editedRecipe.size[0].pulseOffTimeSeconds[0] = 0;
        }
    }

void checkAndFixPulseOffTimes()
    {
    int pulseCount = getNumberOfPulses();
    if (pulseCount != 1)
        {
        for (int i = 0; i < pulseCount - 1; i++)
            {
            if (editedRecipe.size[0].pulseOffTimeSeconds[i] < MIN_PREV_OFF_TIME_FOR_NEW_PULSE)
                {
                editedRecipe.size[0].pulseOffTimeSeconds[i] = MIN_PREV_OFF_TIME_FOR_NEW_PULSE;
                }
            }
        editedRecipe.size[0].pulseOffTimeSeconds[pulseCount - 1] = 0;
        }
    else
        {
        editedRecipe.size[0].pulseOffTimeSeconds[pulseCount - 1] = 0;
        }
    }

void SystemManager::pulseSelectHandler(void)
    {
    if (!pulseSelectInitialized)
        {
        updatePulseSelectMsg();
        myUI->Screen->showMessageNow(&PulseSelectMsg);
        pulseSelectInitialized = true;
        }

    if (releasedTouchValue == TOUCH_NEXT && pulseNumberEdited < NUMBER_OF_PULSES - 1)
        {
        //If the current pulse edit number doesn't have a pulse on time, then do not go to the next pulse
        if (editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] != 0)
            {
            pulseNumberEdited++;
            }
        updatePulseSelectMsg();
        myUI->Screen->showMessageNow(&PulseSelectMsg);
        }
    else if (releasedTouchValue == TOUCH_PREVIOUS && pulseNumberEdited > 0)
        {
        pulseNumberEdited--;
        updatePulseSelectMsg();
        myUI->Screen->showMessageNow(&PulseSelectMsg);
        }
    else if (releasedTouchValue == TOUCH_ACCEPT)
        {
        currentCoffeeBeverageEditState = COFFEE_BEVERAGE_EDIT_SELECTED_PULSE_ON;
        pulseSelectInitialized = false;
        return;
        }
    else if (releasedTouchValue == TOUCH_CANCEL)
        {
        checkAndFixPulseOffTimes();
        CoffeeBeverage* ramRecipes = (CoffeeBeverage*) getCoffeeBeverageTableAddress();

        //FJ added if statement was just [ramRecipes[headSelectIndex] = editedRecipe;]
        // FJ deleted if statement 4/20

        ramRecipes[recipeSelectIndex] = editedRecipe;
        NVBlobs->flushNvBlob(COFFEE_RECIPE_BLOB_INDEX);
        currentCoffeeBeverageEditState = COFFEE_BEVERERAGE_EDITOR_INIT;
        pulseSelectInitialized = false;
        changeState(MENU_BROWSE);
        return;
        }
    }

bool editSelectedPulseInitialized = false;
char editPulseOnTimeSTop[20];
char editPulseOnTimeSBottom[20];
ScreenMessages EditPulseOnTimeSMsg(editPulseOnTimeSTop, editPulseOnTimeSBottom, DEFAULT_STATIC_MENU_PRINT_TIME);

void updateEditSelectedPulseMsg(void)
    {
    if (pulseNumberEdited == 0)
        {
        sprintf(editPulseOnTimeSTop, "PINFUSION ON TME", pulseNumberEdited);
        sprintf(editPulseOnTimeSBottom, "%d SECS", editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited]);
        }
    else
        {
        sprintf(editPulseOnTimeSTop, "PULSE %d ON", pulseNumberEdited);
        sprintf(editPulseOnTimeSBottom, "%d SECS", editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited]);
        }
    }

void SystemManager::editSelectedPulseOnHandler(void)
    {
    if (!editSelectedPulseInitialized)
        {
        updateEditSelectedPulseMsg();
        myUI->Screen->showMessageNow(&EditPulseOnTimeSMsg);
        editSelectedPulseInitialized = true;
        }


    if (releasedTouchValue == TOUCH_NEXT && editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] < MAX_ON_TIME_S_PER_PULSE)
        {
        editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited]++;
        updateEditSelectedPulseMsg();
        myUI->Screen->showMessageNow(&EditPulseOnTimeSMsg);
        }
    else if (holdTouchValue == TOUCH_NEXT && holdTouchTimeMs < STAGE1_TIME_MS && holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
        {
        if (editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] + STAGE1_VALUE < MAX_ON_TIME_S_PER_PULSE)
            {
            editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] += STAGE1_VALUE;
            updateEditSelectedPulseMsg();
            myUI->Screen->showMessageNow(&EditPulseOnTimeSMsg);
            }
        }
    else if (holdTouchValue == TOUCH_NEXT && betweenValues(holdTouchTimeMs, STAGE1_TIME_MS, STAGE2_TIME_MS) && holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
        {
        if (editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] + STAGE2_VALUE < MAX_ON_TIME_S_PER_PULSE)
            {
            editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] += STAGE2_VALUE;
            updateEditSelectedPulseMsg();
            myUI->Screen->showMessageNow(&EditPulseOnTimeSMsg);
            }
        }
    else if (holdTouchValue == TOUCH_NEXT && holdTouchTimeMs >= STAGE2_TIME_MS && holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
        {
        if (editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] + STAGE3_VALUE < MAX_ON_TIME_S_PER_PULSE)
            {
            editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] += STAGE3_VALUE;
            updateEditSelectedPulseMsg();
            myUI->Screen->showMessageNow(&EditPulseOnTimeSMsg);
            }
        }
    else if (releasedTouchValue == TOUCH_PREVIOUS && editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] > 0)
        {
        if (pulseNumberEdited == 0 && editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] > MIN_FIRST_PULSE_ON_TIME_S)
            {
            editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited]--;
            }
        else if (pulseNumberEdited != 0 && editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] > 0)
            {
            editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited]--;
            }
        updateEditSelectedPulseMsg();
        myUI->Screen->showMessageNow(&EditPulseOnTimeSMsg);
        }
    else if (holdTouchValue == TOUCH_PREVIOUS && holdTouchTimeMs < STAGE1_TIME_MS && holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
        {
        if (pulseNumberEdited == 0 && editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] - STAGE1_VALUE > MIN_FIRST_PULSE_ON_TIME_S)
            {
            editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] -= STAGE1_VALUE;
            }
        else if (pulseNumberEdited != 0 && editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] - STAGE1_VALUE > 0)
            {
            editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] -= STAGE1_VALUE;
            }
        updateEditSelectedPulseMsg();
        myUI->Screen->showMessageNow(&EditPulseOnTimeSMsg);
        }
    else if (holdTouchValue == TOUCH_PREVIOUS && betweenValues(holdTouchTimeMs, STAGE1_TIME_MS, STAGE2_TIME_MS) && holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
        {
        if (pulseNumberEdited == 0 && editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] - STAGE2_VALUE > MIN_FIRST_PULSE_ON_TIME_S)
            {
            editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] -= STAGE2_VALUE;
            }
        else if (pulseNumberEdited != 0 && editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] - STAGE2_VALUE > 0)
            {
            editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] -= STAGE2_VALUE;
            }
        updateEditSelectedPulseMsg();
        myUI->Screen->showMessageNow(&EditPulseOnTimeSMsg);
        }
    else if (holdTouchValue == TOUCH_PREVIOUS && holdTouchTimeMs >= STAGE2_TIME_MS && holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
        {
        if (pulseNumberEdited == 0 && editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] - STAGE3_VALUE > MIN_FIRST_PULSE_ON_TIME_S)
            {
            editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] -= STAGE3_VALUE;
            }
        else if (pulseNumberEdited != 0 && editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] - STAGE3_VALUE > 0)
            {
            editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] -= STAGE3_VALUE;
            }
        updateEditSelectedPulseMsg();
        myUI->Screen->showMessageNow(&EditPulseOnTimeSMsg);
        }

    else if (releasedTouchValue == TOUCH_ACCEPT)
        {
        //Recipe cleanup. off time in previous pulse doesn't matter. All additional pulses will be set to zero
        recipeCleanup();
        CoffeeBeverage* ramRecipes = (CoffeeBeverage*) getCoffeeBeverageTableAddress();
        if (machineFeature->numberOfHeads == 1)
            {
            ramRecipes[recipeSelectIndex] = editedRecipe;
            }
        else
            {
            ramRecipes[headSelectIndex] = editedRecipe;
            }
        NVBlobs->flushNvBlob(COFFEE_RECIPE_BLOB_INDEX);

        //Case where pulse on time set to zero
        if (editedRecipe.size[0].pulseOnTimeSeconds[pulseNumberEdited] == 0)
            {
            editEntireRecipeAnswer = false;
            currentCoffeeBeverageEditState = COFFEE_BEVERAGE_SELECT_PULSE;
            }
        else if (pulseNumberEdited == NUMBER_OF_PULSES - 1)
            {
            editEntireRecipeAnswer = false;
            currentCoffeeBeverageEditState = COFFEE_BEVERAGE_SELECT_PULSE;
            }
        else
            {
            currentCoffeeBeverageEditState = COFFEE_BEVERAGE_EDIT_SELECTED_PULSE_OFF;
            }

        editSelectedPulseInitialized = false;
        return;
        }
    else if (releasedTouchValue == TOUCH_CANCEL)
        {
        currentCoffeeBeverageEditState = COFFEE_BEVERAGE_SELECT_PULSE;
        NVBlobs->loadNvBlob(COFFEE_RECIPE_BLOB_INDEX);
        editSelectedPulseInitialized = false;
        editEntireRecipeAnswer = false;
        return;
        }
    }

bool editSelectedPulseOffInitialized = false;
char editPulseOffTimeSTop[20];
char editPulseOffTimeSBottom[20];
ScreenMessages EditPulseOffTimeSMsg(editPulseOffTimeSTop, editPulseOffTimeSBottom, DEFAULT_STATIC_MENU_PRINT_TIME);

void updateEditSelectedPulseOffMsg(void)
    {
    if (pulseNumberEdited == 0)
        {
        sprintf(editPulseOffTimeSTop, "INFUSION OFF TME", pulseNumberEdited);
        sprintf(editPulseOffTimeSBottom, "%d SECS", editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited]);
        }
    else
        {
        sprintf(editPulseOffTimeSTop, "PULSE %d OFF", pulseNumberEdited);
        sprintf(editPulseOffTimeSBottom, "%d SECS", editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited]);
        }
    }

void SystemManager::editSelectedPulseOffHandler(void)
    {
    if (!editSelectedPulseOffInitialized)
        {
        updateEditSelectedPulseOffMsg();
        myUI->Screen->showMessageNow(&EditPulseOffTimeSMsg);
        editSelectedPulseOffInitialized = true;
        }

    if (releasedTouchValue == TOUCH_NEXT && editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited] < MAX_OFF_TIME_S_PER_PULSE)
        {
        editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited]++;
        updateEditSelectedPulseOffMsg();
        myUI->Screen->showMessageNow(&EditPulseOffTimeSMsg);
        }
    else if (holdTouchValue == TOUCH_NEXT && holdTouchTimeMs < STAGE1_TIME_MS && holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
        {
        if (editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited] + STAGE1_VALUE < MAX_OFF_TIME_S_PER_PULSE)
            {
            editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited] += STAGE1_VALUE;
            updateEditSelectedPulseOffMsg();
            myUI->Screen->showMessageNow(&EditPulseOffTimeSMsg);
            }
        }
    else if (holdTouchValue == TOUCH_NEXT && betweenValues(holdTouchTimeMs, STAGE1_TIME_MS, STAGE2_TIME_MS) && holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
        {
        if (editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited] + STAGE2_VALUE < MAX_OFF_TIME_S_PER_PULSE)
            {
            editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited] += STAGE2_VALUE;
            updateEditSelectedPulseOffMsg();
            myUI->Screen->showMessageNow(&EditPulseOffTimeSMsg);
            }
        }
    else if (holdTouchValue == TOUCH_NEXT && holdTouchTimeMs >= STAGE2_TIME_MS && holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
        {
        if (editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited] + STAGE3_VALUE < MAX_OFF_TIME_S_PER_PULSE)
            {
            editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited] += STAGE3_VALUE;
            updateEditSelectedPulseOffMsg();
            myUI->Screen->showMessageNow(&EditPulseOffTimeSMsg);
            }
        }
    else if (releasedTouchValue == TOUCH_PREVIOUS && editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited] > 0)
        {
        editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited]--;
        updateEditSelectedPulseOffMsg();
        myUI->Screen->showMessageNow(&EditPulseOffTimeSMsg);
        }
    else if (holdTouchValue == TOUCH_PREVIOUS && holdTouchTimeMs < STAGE1_TIME_MS && holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
        {
        if (editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited] - STAGE1_VALUE > 0)
            {
            editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited] -= STAGE1_VALUE;
            updateEditSelectedPulseOffMsg();
            myUI->Screen->showMessageNow(&EditPulseOffTimeSMsg);
            }
        }
    else if (holdTouchValue == TOUCH_PREVIOUS && betweenValues(holdTouchTimeMs, STAGE1_TIME_MS, STAGE2_TIME_MS) && holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
        {
        if (editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited] - STAGE2_VALUE > 0)
            {
            editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited] -= STAGE2_VALUE;
            updateEditSelectedPulseOffMsg();
            myUI->Screen->showMessageNow(&EditPulseOffTimeSMsg);
            }
        }
    else if (holdTouchValue == TOUCH_PREVIOUS && holdTouchTimeMs >= STAGE2_TIME_MS && holdTouchTimeMs % BUTTON_HELD_EVENT_SPEED == 0)
        {
        if (editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited] - STAGE3_VALUE > 0)
            {
            editedRecipe.size[0].pulseOffTimeSeconds[pulseNumberEdited] -= STAGE3_VALUE;
            updateEditSelectedPulseOffMsg();
            myUI->Screen->showMessageNow(&EditPulseOffTimeSMsg);
            }
        }

    else if (releasedTouchValue == TOUCH_ACCEPT)
        {
        CoffeeBeverage* ramRecipes = (CoffeeBeverage*) getCoffeeBeverageTableAddress();
        if (machineFeature->numberOfHeads == 1)
            {
            ramRecipes[recipeSelectIndex] = editedRecipe;
            }
        else
            {
            ramRecipes[headSelectIndex] = editedRecipe;
            }
        NVBlobs->flushNvBlob(COFFEE_RECIPE_BLOB_INDEX);
        if (editEntireRecipeAnswer)
            {
            if (pulseNumberEdited < NUMBER_OF_PULSES - 1)
                {
                pulseNumberEdited++;
                currentCoffeeBeverageEditState = COFFEE_BEVERAGE_EDIT_SELECTED_PULSE_ON;
                editSelectedPulseOffInitialized = false;
                return;
                }
            else
                {
                currentCoffeeBeverageEditState = COFFEE_BEVERAGE_SELECT_PULSE;
                }
            }
        else
            {
            currentCoffeeBeverageEditState = COFFEE_BEVERAGE_SELECT_PULSE;
            }
        editSelectedPulseOffInitialized = false;
        return;
        }
    else if (releasedTouchValue == TOUCH_CANCEL)
        {
        currentCoffeeBeverageEditState = COFFEE_BEVERAGE_SELECT_PULSE;
        NVBlobs->loadNvBlob(COFFEE_RECIPE_BLOB_INDEX);
        editSelectedPulseOffInitialized = false;
        editEntireRecipeAnswer = false;
        return;
        }
    }

void SystemManager::stateEditCoffeeRecipeInitialize(void)
    {
    pulseNumberEdited = 0;
    editedRecipe = *getCoffeeBeveragePointer(0);
    if (machineFeature->numberOfHeads == 1)
        {
        recipeSelectorInitialized = false; //FJ Changed from [headSelectIndex = 0;
        currentCoffeeBeverageEditState = COFFEE_BEVERAGE_SELECT_RECIPE; //FJ Changed from [COFFEE_BEVERAGE_SELECT_EDIT_ENTIRE_RECIPE_OR_SPECIFIC_PULSE;                       
        }
    else
        {
        headSelectorInitialized = false;
        currentCoffeeBeverageEditState = COFFEE_BEVERAGE_SELECT_HEAD;
        }
    }

void SystemManager::stateEditCoffeeRecipeMain(void)
    {
    switch (currentCoffeeBeverageEditState)
        {
        case COFFEE_BEVERERAGE_EDITOR_INIT:
            stateEditCoffeeRecipeInitialize();
            break;
        case COFFEE_BEVERAGE_SELECT_RECIPE: //FJ added
            recipeSelectHandler();
            break;
        case COFFEE_BEVERAGE_SELECT_HEAD:
            headSelectHandler();
            break;
        case COFFEE_BEVERAGE_SELECT_EDIT_ENTIRE_RECIPE_OR_SPECIFIC_PULSE:
            editEntireRecipeHandler();
            break;
        case COFFEE_BEVERAGE_SELECT_PULSE:
            pulseSelectHandler();
            break;
        case COFFEE_BEVERAGE_EDIT_SELECTED_PULSE_ON:
            editSelectedPulseOnHandler();
            break;
        case COFFEE_BEVERAGE_EDIT_SELECTED_PULSE_OFF:
            editSelectedPulseOffHandler();
            break;
        }
    }
