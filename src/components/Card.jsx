export default function Card(props) {
  return (
    <div className="container flex gap-4 w-full py-5 rounded-3xl bg-gradient-to-r from-indigo-600 to-indigo-500">
      <div className="flex items-center">
        <img
          alt="logo"
          src={props.logo}
          className="w-[40px] rounded-full bg-white p-1"
        />
      </div>
      <div className="content ">
        <h1 className="text-xl text-white font-bold">{props.title}</h1>
        <p className="text-white font-thin">{props.value}</p>
      </div>
    </div>
  );
}
