// GBTournamentView.cpp
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include "GBTournamentView.h"
#include "GBSide.h"
#include "GBWorld.h"
#include "GBStringUtilities.h"


const short kNameLeft = 25;
const short kPercentRight = 200;
const short kSurvivalRight = kPercentRight + 50;
const short kEarlyDeathRight = kSurvivalRight + 35;
const short kLateDeathRight = kEarlyDeathRight + 35;
const short kEarlyScoreRight = kLateDeathRight + 40;
const short kFractionRight = kEarlyScoreRight + 40;
const short kKillsRight = kFractionRight + 50;
const short kRoundsRight = kKillsRight + 40;
const short kWidth = kRoundsRight + 10;

GBColor GBTournamentView::RangeColor(float value, float min, float max,
		const GBColor & low, const GBColor & high,
		long rounds, long minrounds) {
	if ( rounds < minrounds ) return GBColor::gray;
	if ( value < min ) return low;
	if ( value > max ) return high;
	return GBColor::black;
}

GBTournamentView::GBTournamentView(GBWorld & target)
	: world(target), lastRounds(-1)
{}

void GBTournamentView::Draw() {
	GBListView::Draw();
// record
	lastRounds = world.TournamentScores().Rounds();
}

bool GBTournamentView::InstantChanges() const {
	return world.TournamentScores().Rounds() != lastRounds;
}

short GBTournamentView::PreferredWidth() const {
	return kWidth;
}

const string GBTournamentView::Name() const {
	return "Tournament";
}

long GBTournamentView::Items() const {
	return world.CountSides();
}

short GBTournamentView::HeaderHeight() const {
	return 24;
}

short GBTournamentView::ItemHeight() const {
	return 15;
}

short GBTournamentView::FooterHeight() const {
	return 15;
}

void GBTournamentView::DrawHeader(const GBRect & box) {
	DrawBox(box);
	DrawStringLeft("Side", box.left + kNameLeft, box.bottom - 3, 10, GBColor::black);
// draw various numbers
	DrawStringRight("Score", box.left + kPercentRight, box.bottom - 3, 10, GBColor::black, true);
	DrawStringRight("Survival", box.left + kSurvivalRight, box.bottom - 3, 10, GBColor::black);
	DrawStringRight("Death rates:", box.left + kLateDeathRight - 5, box.bottom - 13, 10, GBColor::black);
	DrawStringRight("Early", box.left + kEarlyDeathRight, box.bottom - 3, 10, GBColor::black);
	DrawStringRight("Late", box.left + kLateDeathRight, box.bottom - 3, 10, GBColor::black);
	DrawStringRight("Early", box.left + kEarlyScoreRight, box.bottom - 13, 10, GBColor::black);
	DrawStringRight("Score", box.left + kEarlyScoreRight, box.bottom - 3, 10, GBColor::black);
	DrawStringRight("Fraction", box.left + kFractionRight + 10, box.bottom - 3, 10, GBColor::black);
	DrawStringRight("Kill", box.left + kKillsRight, box.bottom - 13, 10, GBColor::black);
	DrawStringRight("rate", box.left + kKillsRight, box.bottom - 3, 10, GBColor::black);
	DrawStringRight("Rounds", box.left + kRoundsRight, box.bottom - 3, 10, GBColor::black);
}

void GBTournamentView::DrawItem(long index, const GBRect & box) {
	DrawBox(box);
	const GBSide * side = world.GetSide(index);
	if ( ! side ) return;
// draw ID and name
	DrawStringRight(ToString(side->ID()) + '.', box.left + kNameLeft - 5, box.bottom - 4, 10, side->Color());
	DrawStringLeft(side->Name(), box.left + kNameLeft, box.bottom - 4, 10, GBColor::black);
// draw various numbers
	long rounds = side->TournamentScores().Rounds();
	long survived = side->TournamentScores().Survived();
	long notearly = rounds - side->TournamentScores().EarlyDeaths();
	if ( rounds > 0 ) {
		GBNumber score = side->TournamentScores().BiomassFraction();
		DrawStringRight(ToPercentString(score, 1), box.left + kPercentRight, box.bottom - 4,
			10, rounds + survived < kMinColorRounds * 2 ? GBColor::gray : GBColor::black, true);
		float survival = side->TournamentScores().SurvivalNotSterile();
		DrawStringRight(ToPercentString(survival),
			box.left + kSurvivalRight, box.bottom - 4,
			10, RangeColor(survival, 0.2, 0.4, GBColor::darkRed, GBColor::darkGreen, rounds));
		float early = side->TournamentScores().EarlyDeathRate();
		DrawStringRight(ToPercentString(early, 0),
			box.left + kEarlyDeathRight, box.bottom - 4,
			10, RangeColor(early, 0.2, 0.4, GBColor::darkGreen, GBColor::darkRed, rounds));
	}
	if ( notearly > 0 ) {
		float late = side->TournamentScores().LateDeathRate();
		DrawStringRight(ToPercentString(late, 0),
			box.left + kLateDeathRight, box.bottom - 4,
			10, RangeColor(late, 0.4, 0.6, GBColor::darkGreen, GBColor::darkRed, notearly));
	}
	if ( rounds > 0 ) {
		GBNumber early = side->TournamentScores().EarlyBiomassFraction();
		DrawStringRight(ToPercentString(early), box.left + kEarlyScoreRight, box.bottom - 4,
			10, RangeColor(early.ToDouble(), 0.08, 0.12, GBColor::darkRed, GBColor::darkGreen,
				rounds + notearly, kMinColorRounds * 2));
	}
	if ( survived > 0 ) {
		float fraction = side->TournamentScores().SurvivalBiomassFraction();
		DrawStringRight(ToPercentString(fraction, 0),
			box.left + kFractionRight, box.bottom - 4,
			10, RangeColor(fraction, 0.2, 0.4, GBColor::blue, GBColor::purple, survived));
	}
	if ( rounds > 0 ) {
		float kills = side->TournamentScores().KillRate();
		DrawStringRight(ToPercentString(kills, 0), box.left + kKillsRight, box.bottom - 4,
			10, RangeColor(kills, 1.0, 2.0, GBColor::blue, GBColor::purple, survived));
	}
	DrawStringRight(ToString(rounds), box.left + kRoundsRight, box.bottom - 4,
			10, rounds < kMinColorRounds ? GBColor::darkRed : GBColor::blue);
}

void GBTournamentView::DrawFooter(const GBRect & box) {
	DrawBox(box);
	DrawStringLeft("Overall:", box.left + kNameLeft, box.bottom - 4, 10, GBColor::black, true);
// draw various numbers
	long rounds = world.TournamentScores().Rounds();
	long notearly = world.TournamentScores().SurvivedEarly();
	if ( rounds > 0 ) {
		float survival = world.TournamentScores().SurvivalNotSterile();
		DrawStringRight(ToPercentString(survival, 0), box.left + kSurvivalRight, box.bottom - 4,
			10, RangeColor(survival, 0.25, 0.5, GBColor::darkRed, GBColor::darkGreen, rounds));
		float early = world.TournamentScores().EarlyDeathRate();
		DrawStringRight(ToPercentString(early, 0),
			box.left + kEarlyDeathRight, box.bottom - 4,
			10, RangeColor(early, 0.2, 0.4, GBColor::darkGreen, GBColor::darkRed, rounds));
	}
	if ( notearly > 0 ) {
		float late = world.TournamentScores().LateDeathRate();
		DrawStringRight(ToPercentString(late, 0), box.left + kLateDeathRight, box.bottom - 4,
			10, RangeColor(late, 0.45, 0.6, GBColor::darkGreen, GBColor::darkRed, rounds));
	}
	if ( rounds > 0 ) {
		float kills = world.TournamentScores().KillRate();
		DrawStringRight(ToPercentString(kills, 0), box.left + kKillsRight, box.bottom - 4,
			10, RangeColor(kills, 1.2, 1.8, GBColor::blue, GBColor::purple, rounds));
	}
	DrawStringRight(ToString(rounds), box.left + kRoundsRight, box.bottom - 4,
			10, rounds < kMinColorRounds ? GBColor::darkRed : GBColor::blue);
}

