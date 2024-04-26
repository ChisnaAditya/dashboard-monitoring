export default function MyInput(props) {
  return (
    <div>
      <label
        htmlFor={props.for}
        className="block text-sm font-medium leading-6 text-gray-900"
      >
        {props.inputName}
      </label>
      <div className="mt-2">
        <input
          id={props.id}
          name={props.name}
          type={props.type}
          onChange={props.onChange}
          required
          className="block w-full rounded-md border-0 py-1.5 px-2 text-gray-900 text-sm shadow-sm ring-1 ring-inset ring-gray-300 placeholder:text-gray-400 focus:ring-2 focus:ring-inset focus:ring-indigo-600 sm:text-sm sm:leading-6"
        />
      </div>
    </div>
  );
}
