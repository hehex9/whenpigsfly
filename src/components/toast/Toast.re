Import.style("./style.styl");

type state = bool;

type action =
  | Mounted;

let component = React.reducerComponent("Toast");

/* let id = "app-toast"; */

let make = (~visible=false, ~className="", children) => {
  ...component,

  initialState: () => false,

  reducer: (action, _state) =>
    switch (action) {
    | Mounted => React.Update(true)
    },

  didMount: self => self.send(Mounted),

  render: ({state}) =>
    state ?
      switch (Webapi.Dom.(document |> Document.querySelector("body"))) {
      | None => React.null
      | Some(el) =>
        ReactDOMRe.createPortal(
          <div
            className={
              "app-toast " ++ className ++ (visible ? " visible" : "")
            }>
            children
          </div>,
          el,
        )
      } :
      React.null,
};