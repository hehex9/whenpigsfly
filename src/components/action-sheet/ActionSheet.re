Import.style("./style.styl");

let component = React.statelessComponent("ActionSheet");

let make =
    (
      ~title="",
      ~visible=false,
      ~data: array(string),
      ~onClick,
      ~onCancel,
      _children,
    ) => {
  ...component,

  render: _self => {
    let items =
      Array.mapi(
        (idx, item) =>
          <div
            className={
              "action-sheet-item" ++ (idx == 0 ? "" : " hairline hairline-top")
            }
            onClick={_e => onClick(idx)}
            key={string_of_int(idx)}>
            {React.string(item)}
          </div>,
        data,
      );
    let cls = "action-sheet-container " ++ (visible ? "active" : "");

    <div className=cls>
      <div className="action-sheet-overlay" onClick=onCancel />
      <div className="action-sheet">
        {
          String.length(title) > 0 ?
            <div className="action-sheet-title">
              {React.string(title)}
            </div> :
            React.null
        }
        {React.array(items)}
        <div
          className="action-sheet-item action-sheet-item-cancel"
          onClick=onCancel>
          {React.string({js|取消|js})}
        </div>
      </div>
    </div>;
  },
};