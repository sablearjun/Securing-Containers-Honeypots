import AlertStatus from "../modules/alerts/alert-status";
import ThreatHuntSelection from "../modules/threathunting-tactics/threathunt-selection";

const ThreathuntingTactics = () => {
  return (
    <>
      <div className="card card-body">
          <AlertStatus/>
      </div>
      <div className="card py-3">
          <ThreatHuntSelection/>
      </div>
    </>
  );
};

export default ThreathuntingTactics;
