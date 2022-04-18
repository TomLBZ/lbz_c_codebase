#include "curses.h"

bool initColors(){
    bool hasColor = has_colors();
	if(hasColor == true){
		start_color();			// start the color functionality
		init_pair(C_BLACK_BLACK		, COLOR_BLACK   , COLOR_BLACK   );
		init_pair(C_BLUE_BLACK		, COLOR_BLUE    , COLOR_BLACK   );
		init_pair(C_CYAN_BLACK		, COLOR_CYAN    , COLOR_BLACK   );
		init_pair(C_GREEN_BLACK 	, COLOR_GREEN   , COLOR_BLACK   );
		init_pair(C_MAGENTA_BLACK	, COLOR_MAGENTA , COLOR_BLACK   );
		init_pair(C_RED_BLACK		, COLOR_RED     , COLOR_BLACK   );
		init_pair(C_WHITE_BLACK		, COLOR_WHITE   , COLOR_BLACK   );
		init_pair(C_YELLOW_BLACK	, COLOR_YELLOW  , COLOR_BLACK   );

		init_pair(C_BLACK_BLUE		, COLOR_BLACK   , COLOR_BLUE    );
		init_pair(C_BLUE_BLUE		, COLOR_BLUE    , COLOR_BLUE    );
		init_pair(C_CYAN_BLUE		, COLOR_CYAN    , COLOR_BLUE    );
		init_pair(C_GREEN_BLUE		, COLOR_GREEN   , COLOR_BLUE    );
		init_pair(C_MAGENTA_BLUE	, COLOR_MAGENTA , COLOR_BLUE    );
		init_pair(C_RED_BLUE		, COLOR_RED     , COLOR_BLUE    );
		init_pair(C_WHITE_BLUE		, COLOR_WHITE   , COLOR_BLUE    );
		init_pair(C_YELLOW_BLUE		, COLOR_YELLOW  , COLOR_BLUE    );

		init_pair(C_BLACK_CYAN		, COLOR_BLACK   , COLOR_CYAN    );
		init_pair(C_BLUE_CYAN		, COLOR_BLUE    , COLOR_CYAN    );
		init_pair(C_CYAN_CYAN		, COLOR_CYAN    , COLOR_CYAN    );
		init_pair(C_GREEN_CYAN		, COLOR_GREEN   , COLOR_CYAN    );
		init_pair(C_MAGENTA_CYAN	, COLOR_MAGENTA , COLOR_CYAN    );
		init_pair(C_RED_CYAN		, COLOR_RED     , COLOR_CYAN    );
		init_pair(C_WHITE_CYAN		, COLOR_WHITE   , COLOR_CYAN    );
		init_pair(C_YELLOW_CYAN		, COLOR_YELLOW  , COLOR_CYAN    );

		init_pair(C_BLACK_GREEN		, COLOR_BLACK   , COLOR_GREEN   );
		init_pair(C_BLUE_GREEN		, COLOR_BLUE    , COLOR_GREEN   );
		init_pair(C_CYAN_GREEN		, COLOR_CYAN    , COLOR_GREEN   );
		init_pair(C_GREEN_GREEN		, COLOR_GREEN   , COLOR_GREEN   );
		init_pair(C_MAGENTA_GREEN	, COLOR_MAGENTA , COLOR_GREEN   );
		init_pair(C_RED_GREEN		, COLOR_RED     , COLOR_GREEN   );
		init_pair(C_WHITE_GREEN		, COLOR_WHITE   , COLOR_GREEN   );
		init_pair(C_YELLOW_GREEN	, COLOR_YELLOW  , COLOR_GREEN   );

		init_pair(C_BLACK_MAGENTA	, COLOR_BLACK   , COLOR_MAGENTA );
		init_pair(C_BLUE_MAGENTA	, COLOR_BLUE    , COLOR_MAGENTA );
		init_pair(C_CYAN_MAGENTA	, COLOR_CYAN    , COLOR_MAGENTA );
		init_pair(C_GREEN_MAGENTA	, COLOR_GREEN   , COLOR_MAGENTA );
		init_pair(C_MAGENTA_MAGENTA	, COLOR_MAGENTA , COLOR_MAGENTA );
		init_pair(C_RED_MAGENTA		, COLOR_RED     , COLOR_MAGENTA );
		init_pair(C_WHITE_MAGENTA	, COLOR_WHITE   , COLOR_MAGENTA );
		init_pair(C_YELLOW_MAGENTA	, COLOR_YELLOW  , COLOR_MAGENTA );

		init_pair(C_BLACK_RED		, COLOR_BLACK   , COLOR_RED     );
		init_pair(C_BLUE_RED		, COLOR_BLUE    , COLOR_RED     );
		init_pair(C_CYAN_RED		, COLOR_CYAN    , COLOR_RED     );
		init_pair(C_GREEN_RED		, COLOR_GREEN   , COLOR_RED     );
		init_pair(C_MAGENTA_RED		, COLOR_MAGENTA , COLOR_RED     );
		init_pair(C_RED_RED			, COLOR_RED     , COLOR_RED     );
		init_pair(C_WHITE_RED		, COLOR_WHITE   , COLOR_RED     );
		init_pair(C_YELLOW_RED		, COLOR_YELLOW  , COLOR_RED     );

		init_pair(C_BLACK_WHITE		, COLOR_BLACK   , COLOR_WHITE   );
		init_pair(C_BLUE_WHITE		, COLOR_BLUE    , COLOR_WHITE   );
		init_pair(C_CYAN_WHITE		, COLOR_CYAN    , COLOR_WHITE   );
		init_pair(C_GREEN_WHITE		, COLOR_GREEN   , COLOR_WHITE   );
		init_pair(C_MAGENTA_WHITE	, COLOR_MAGENTA , COLOR_WHITE   );
		init_pair(C_RED_WHITE		, COLOR_RED     , COLOR_WHITE   );
		init_pair(C_WHITE_WHITE		, COLOR_WHITE   , COLOR_WHITE   );
		init_pair(C_YELLOW_WHITE	, COLOR_YELLOW  , COLOR_WHITE   );

		init_pair(C_BLACK_YELLOW	, COLOR_BLACK   , COLOR_YELLOW  );
		init_pair(C_BLUE_YELLOW		, COLOR_BLUE    , COLOR_YELLOW  );
		init_pair(C_CYAN_YELLOW		, COLOR_CYAN    , COLOR_YELLOW  );
		init_pair(C_GREEN_YELLOW	, COLOR_GREEN   , COLOR_YELLOW  );
		init_pair(C_MAGENTA_YELLOW	, COLOR_MAGENTA , COLOR_YELLOW  );
		init_pair(C_RED_YELLOW		, COLOR_RED     , COLOR_YELLOW  );
		init_pair(C_WHITE_YELLOW	, COLOR_WHITE   , COLOR_YELLOW  );
		init_pair(C_YELLOW_YELLOW	, COLOR_YELLOW  , COLOR_YELLOW  );
	}
    return hasColor;
}