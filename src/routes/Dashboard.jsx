import Card from "../components/Card";
import RELAY from "../assets/relay.svg";
import SUHU from "../assets/suhu.svg";
import DO from "../assets/do.svg";
import PH from "../assets/ph.svg";
import USER from "../assets/user.svg";
export default function Dashboard() {
  return (
    <div>
      <div className="container flex items-center justify-evenly h-24 bg-pattern rounded-b-[40px] shadow-xl">
        <h1 className="text-4xl text-black font-bold">Dashboard</h1>
        <div className="user">
          <img
            alt="user"
            src={USER}
            className="w-[40px] rounded-full bg-indigo-600 p-1"
          />
        </div>
      </div>
      <section className="container py-5">
        <div className="pb-2 font-bold px-2">Sensors</div>
        <div className="flex flex-col gap-4">
          <Card logo={SUHU} title="title" value="value" />
          <Card logo={DO} title="title" value="value" />
          <Card logo={PH} title="title" value="value" />
        </div>
      </section>
      <section className="container py-2">
        <div className="pb-2 font-bold px-2">Control</div>
        <div className="flex flex-col gap-4">
          <Card logo={RELAY} title="Relay Status" value="OFF" />
        </div>
      </section>
    </div>
  );
}
